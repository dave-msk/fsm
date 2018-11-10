#ifndef FSM_CORE_STATE_H
#define FSM_CORE_STATE_H
#include <map>
#include <string>
#include <sstream>
#include "exceptions.h"

namespace fsm {

template <typename Op>
class State {
 private:
  std::map<Op, std::string> tr_map_;
  std::string id_;
  bool is_terminal_;
 public:
  State() : is_terminal_(false) {};
  State(const std::string& id, const bool is_terminal)
      : id_(id), is_terminal_(is_terminal) {};

  bool IsTerminal();
  bool HasOp(const Op &op) const;
  void AddOp(const Op &op, const std::string &state_id);

  const std::string GetId() const;
  const std::string& Transit(const Op& op) const;

};

template <typename Op>
void State<Op>::AddOp(const Op &op, const std::string &state_id) {
  auto it = tr_map_.find(op);
  if (it != tr_map_.end()) {
    std::stringstream ss;
    ss << "Duplicated transition key :\"" << op << "\"" << std::endl;
    throw fsm::DuplicatedKeyException(ss.str());
  }
  tr_map_[op] = state_id;
}


template <typename Op>
const std::string State<Op>::GetId() const {
  return id_;
}


template <typename Op>
bool State<Op>::IsTerminal() {
  return is_terminal_;
}

template <typename Op>
const std::string& State<Op>::Transit(const Op& op) const {
  auto it = tr_map_.find(op);
  if (it == tr_map_.end()) {
    std::stringstream ss;
    ss << "Transition key \"" << op << "\" not found." << std::endl;
    throw fsm::InvalidTransitionException(ss.str());
  }
  return it->second;
}

template <typename Op>
bool State<Op>::HasOp(const Op &op) const {
  auto it = tr_map_.find(op);
  return it != tr_map_.end();
}

}

#endif  // FSM_CORE_STATE_H
