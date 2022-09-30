#pragma once

#include <cstdint>
#include <frg/macros.hpp>
#include <sys/types.h> // ssize_t

namespace frg FRG_VISIBILITY {
template <typename T> class intrusive_queue_item;

enum class QueueResult : uint8_t { ENQUEUE_FAIL, ENQUEUE_OKAY };

template <typename T> class intrusive_queue {
private:
  static constexpr ssize_t limit = -1;
  size_t _size{};

  // [back] ||||||||| [front]
  T _back{nullptr}, _front{nullptr};

public:
  struct Element {
  public:
    T *next{nullptr};
    T *prev{nullptr};
  };

  inline QueueResult enqueue(T &item) {
    if (item == nullptr || item->item_in_queue())
      return QueueResult::ENQUEUE_FAIL;

    // Did we impose a size limit on NonAllocQueues'?
    if constexpr (limit != -1)
      if (_size > limit)
        return QueueResult::ENQUEUE_FAIL;

    if (_back == nullptr)
      _front = _back = item;

    _back->next = item;
    _back = item;
    _size++;

    return QueueResult::ENQUEUE_OKAY;
  }

  inline T dequeue() {
    if (empty())
      return T();

    auto node = _front;
    _front = _front->next;

    if (!_front)
      _back = nullptr;

    return node;
  }

  inline size_t size() const { return _size; }
  inline bool empty() const { return _front == nullptr; }
  inline T front() const { return _front; }
  inline T back() const { return _back; }
};

} // namespace FRG_VISIBILITY