#include <thread>
#include <condition_variable>
#include <mutex>
#include <functional>
#include <vector>
#include <queue>
#include <future>

class ThreadPool
{
public:
  // Constructor
  explicit ThreadPool(std::size_t nunThreads = 1)
  {
    start(nunThreads);
  }

  // Destructor
  ~ThreadPool()
  {
    stop();
  }

  // Encolar tarea en el pool
  template <class T, class... Args>
  auto enqueue(T &&task, Args &&...args) -> std::future<decltype(task(args...))>
  {
    using ReturnType = decltype(task(args...));
    auto wrapper = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<T>(task), std::forward<Args>(args)...));
    {
      std::unique_lock<std::mutex> lock{eventMutex};
      nTasks.emplace([=]()
                     { (*wrapper)(); });
    }
    eventVar.notify_one();
    return wrapper->get_future();
  }

private:
  using Task = std::function<void()>; // Alias Task: std::function<void()>

  // Atributos
  std::vector<std::thread> nThreads; // Vector de threads
  std::condition_variable eventVar;  // Variable de condicion
  std::mutex eventMutex;             // Mutex
  bool stopping = false;             // Bandera de parada
  std::queue<Task> nTasks;           // Cola de tareas

  // Metodos

  void start(std::size_t nunThreads)
  {
    for (auto i = 0u; i < nunThreads; ++i)
    {
      nThreads.emplace_back([=]()
                            {
          while (true)
          {
            Task task;
            {
              std::unique_lock<std::mutex> lock{eventMutex};
              eventVar.wait(lock, [=]() { return stopping || !nTasks.empty(); });
              if (stopping && nTasks.empty())
              {
                break;
              }
              task = std::move(nTasks.front());
              nTasks.pop();
            }
            task();
          } });
    }
  }

  void stop() noexcept
  {
    {
      std::unique_lock<std::mutex> lock{eventMutex};
      stopping = true;
    }
    eventVar.notify_all();
    for (auto &thread : nThreads)
    {
      thread.join();
    }
  }
};