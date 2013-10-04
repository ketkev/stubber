/*
 * stub.h
 *
 *  Created on: 21.04.2013
 *      Author: scn
 */

#ifndef STUBBER_H_
#define STUBBER_H_

// project includes
#include "any.h"

// system includes
#include <utility>
#include <tuple>
#include <list>
#include <map>
#include <initializer_list>
#include <sstream>
#include <ostream>
#include <stdexcept>

#include <iostream>


class stubber;


extern stubber s_stub;


class stubber {
  public:


  class function_call {
    public:


    class argument {
      public:


      explicit argument() :
          m_value(std::string("")) {
      }
      template<typename T>
      argument(T const & value) {
        std::ostringstream oss;
        oss << value;
        m_value = oss.str();
      }
      ~argument() = default;
      argument(argument const & right) = default;
      argument(argument&& right) {
        m_value = std::move(right.m_value);
      }

      argument & operator =(argument const & right) {
        m_value = right.m_value;
        return *this;
      }

      template<class T>
      friend inline bool operator ==(argument const & left, T const & right) {
        return left == argument(right);
      }

      friend inline bool operator ==(argument const & left,
          argument const & right) {
        return left.m_value == right.m_value;
      }

      friend inline bool operator ==(argument const & left,
          std::string const & right) {
        return left.m_value == right;
      }

      template<typename T, typename S>
      friend inline bool operator !=(T const & left, S const & right) {
        return !(left == right);
      }

      friend inline std::ostream & operator <<(std::ostream & os,
          argument const & arg) {
        os << arg.m_value;
        return os;
      }


      private:


      std::string m_value;
    };
    typedef std::map<std::string, argument> t_argument_map;
    typedef std::pair<std::string, argument> t_name_argument;

    function_call(std::string const & name, std::initializer_list<t_name_argument> const & arguments)
      :  m_name(name), m_arguments(t_argument_map())
    {
      for (auto const & item : arguments) {
        m_arguments.insert(item);
      }
    }
    ~function_call() = default;
    function_call(function_call const &) = default;
    function_call(function_call&& right) {
      m_name = std::move(right.m_name);
      m_arguments = std::move(right.m_arguments);
    }
    ;

    function_call& operator =(function_call& right) {
      m_name = right.m_name;
      m_arguments = right.m_arguments;

      return *this;
    }

    std::string const name() const {
      return m_name;
    }
    t_argument_map const & arguments() const {
      return m_arguments;
    }

    argument const & param(std::string const & name) {
      return m_arguments[name];
    }

    argument const & operator ()(std::string const & name) {
      return m_arguments[name];
    }


    private:


    std::string m_name;
    t_argument_map m_arguments;
  };
  typedef std::list<function_call> t_function_call_list;
  typedef std::map<std::string, any_abstract*> t_function_results;
  typedef std::map<std::string, std::map<std::string, any_abstract*> > t_function_parameter_returns;

  stubber() :
    m_function_calls(t_function_call_list()),
    m_function_results(t_function_results()),
    m_function_parameter_returns(t_function_parameter_returns())
  {
  }
  ~stubber() = default;

  stubber(stubber&) = delete;
  stubber(stubber&&) = delete;

  static stubber * get_stub() {
    return &s_stub;
  }

  void reset() {
    m_function_calls.clear();
    m_function_results.clear();
    m_function_parameter_returns.clear();
  }

  void register_call(std::string const & name, std::initializer_list<function_call::t_name_argument> const & arguments) {
    m_function_calls.push_back(function_call(name, arguments));
  }

  template <class T>
  void register_function_result(std::string const & function_name, T result) {
    m_function_results[function_name] = (any_abstract*)new any<T>(result);
  }

  template <class T>
  void register_function_parameter_return(std::string const & function_name, std::string const & parameter_name, T value) {
    m_function_parameter_returns[function_name][parameter_name] = (any_abstract*)new any<T>(value);
  }

  template <class T>
  T get_result(std::string const & function_name) {
    T result;
    try {
      any_abstract* something = m_function_results.at(function_name);
      result = *(reinterpret_cast<T*>(something->get_value()));
    } catch (std::out_of_range const & e) {
      throw std::runtime_error("No result defined for '" + function_name + "(..)'");
    }
    return result;
  }

  template <class T>
  T get_function_parameter_return(std::string const & function_name, std::string const & parameter_name) {
    T result;
    try {
      auto parameters = m_function_parameter_returns.at(function_name);
      auto value = parameters.at(parameter_name);
      result = *(reinterpret_cast<T*>(value->get_value()));
    } catch (std::out_of_range const & e) {
      throw std::runtime_error("No result defined for '" + function_name + "(..)', parameter: '" + parameter_name + "'");
    }
    return result;
  }

  t_function_call_list const & function_calls() const {
    return m_function_calls;
  }


  private:


  t_function_call_list m_function_calls;
  t_function_results m_function_results;
  t_function_parameter_returns m_function_parameter_returns;
};

typedef stubber::function_call::argument t_arg;


#endif /* STUBBER_H_ */

