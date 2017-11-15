# stack0.0.6

- Сделать класс `stack` более
  - потокодружелюбным (устранив простои) `// std::condition_variable`
  - дружелюбным относительно исключений (убрав генерацию исключения) `// pop -> try_pop + wait_and_pop`
```
template <typename T>
class stack /*thread-safe*/
{
public:
  stack();
  size_t count() const;
  void push(T const &);
  auto try_pop() -> std::shared_ptr<T>;
  auto wait_and_pop() -> std::shared_ptr<T>;
private:
  T * ptr_;
  size_t size_;
  size_t count_;
  std::mutex mutex_;
  std::condition_variable cond_;
};
```
