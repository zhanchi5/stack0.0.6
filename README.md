# stack0.0.5

- Сделать класс `stack` потокобезопасным,  устранив:
  - гонку за данными, используя объект типа `std::mutex`
  - архитектурную ошибку, объеденив методы `pop`  и `top` в один `pop`
```
template <typename T>
class stack /*thread-safe*/
{
public:
  stack();
  size_t count() const;
  void push(T const &);
  auto pop() -> std::shared_ptr<T>;
private:
  swap( stack & other )
  T * array_;
  size_t array_size_;
  size_t count_;
};
```
