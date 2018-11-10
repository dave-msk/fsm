#ifndef FSM_CORE_EXCEPTIONS_H
#define FSM_CORE_EXCEPTIONS_H
#include <exception>
#include <string>

namespace fsm {

struct Exception : std::exception {
 private:
  std::string message_;
 public:
  explicit Exception(const std::string& message)
      : message_(message) {};

  const char *what() const noexcept override {
    return message_.c_str();
  }
};

struct DuplicatedKeyException : public Exception {
  using Exception::Exception;
};

struct InvalidTransitionException : public Exception {
  using Exception::Exception;
};

}

#endif  // FSM_CORE_EXCEPTIONS_H
