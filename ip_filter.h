/*!
\file
\brief Объявление класса IpFilter и вспомогательный структур и функций
*/

#pragma once

#include <vector>
#include <functional>

template<typename T> using UnaryPredicate = std::function<bool(T)>;

template <typename> class IpFilter;

/*!
\brief Шаблонный функтор, предоставляющий интерфейс бинарного предиката.
Всегда возвращает true и используется для операции сравнения части ip
адреса в классе IpFilter по умолчанию.
*/
template<typename T>
struct any_value
{
  constexpr bool operator()(const T &lhs, const T &rhs) const {
    return true;
  }
};

/*!
\brief Создает лямбду функцию с интерфейсом унарного предиката из входной функции
бинарного предиката и одного аргумента для него. Лямбда захватывает данные по значению.
Функция являетсяконструктором операций сравнения для части ip адреса.
\param[in] BinaryPredicate Бинарный предикат.
\param[in] comparer_value Одно из значений для сравнения бинарным предикатом.
\return UnaryPredicate<T> Лямбда функция с созраненным бинарноым предикатом и одним значением для сравнения. 
*/
template<typename BinaryPredicate, typename T>
UnaryPredicate<T> make_ip_part_comparator(const BinaryPredicate& pred, const T& comparer_value)
{
  return
    [=](const T& ip_part)
    {
      return pred(ip_part, comparer_value);
    };
}

/*!
\brief Переназначений операций сравнения частей ip адреса для уже сконструированного экземпляра IpFilter.
По умолчанию передаются операции сравнения всегда возвращающие true.
\param[in] ip_filter Экземпляр IpFilter, для которого будут переназначены операции сравнения частей ip адреса.
\param[in] ip_part_comparator1 Унарный предикат для сравнения первой части ip адреса, созданный с помощью make_ip_part_comparator.
\param[in] ip_part_comparator2 Унарный предикат для сравнения второй части ip адреса, созданный с помощью make_ip_part_comparator.
\param[in] ip_part_comparator3 Унарный предикат для сравнения третьей части ip адреса, созданный с помощью make_ip_part_comparator.
\param[in] ip_part_comparator4 Унарный предикат для сравнения четвертой части ip адреса, созданный с помощью make_ip_part_comparator.
\return IpFilter<T> Экземпляр класса IpFilter. 
*/
template<typename T>
void reinit_ip_filter(
  IpFilter<T>& ip_filter,
  UnaryPredicate<T>&& ip_part_comparator1 = make_ip_part_comparator(any_value<T>(), T()),
  UnaryPredicate<T>&& ip_part_comparator2 = make_ip_part_comparator(any_value<T>(), T()),
  UnaryPredicate<T>&& ip_part_comparator3 = make_ip_part_comparator(any_value<T>(), T()),
  UnaryPredicate<T>&& ip_part_comparator4 = make_ip_part_comparator(any_value<T>(), T()))
{
  ip_filter.clear_comparators();
  ip_filter.add_ip_part_comparator(
    std::forward<UnaryPredicate<T>>(ip_part_comparator1),
    std::forward<UnaryPredicate<T>>(ip_part_comparator2),
    std::forward<UnaryPredicate<T>>(ip_part_comparator3),
    std::forward<UnaryPredicate<T>>(ip_part_comparator4));
}

/*!
\brief Констрктор экземпляра IpFilter. Используется для автоматического вывода типа, а также
для корректного создания экземпляра, т.к. необходимо передать 4 операции сравнения со значениями
для сравнения. По умолчанию передаются операции сравнения всегда возвращающие true.
\param[in] ip_part_comparator1 Унарный предикат для сравнения первой части ip адреса, созданный с помощью make_ip_part_comparator.
\param[in] ip_part_comparator2 Унарный предикат для сравнения второй части ip адреса, созданный с помощью make_ip_part_comparator.
\param[in] ip_part_comparator3 Унарный предикат для сравнения третьей части ip адреса, созданный с помощью make_ip_part_comparator.
\param[in] ip_part_comparator4 Унарный предикат для сравнения четвертой части ip адреса, созданный с помощью make_ip_part_comparator.
\return IpFilter<T> Экземпляр класса IpFilter. 
*/
template<typename T>
IpFilter<T> make_ip_filter(
  UnaryPredicate<T>&& ip_part_comparator1 = make_ip_part_comparator(any_value<T>(), T()),
  UnaryPredicate<T>&& ip_part_comparator2 = make_ip_part_comparator(any_value<T>(), T()),
  UnaryPredicate<T>&& ip_part_comparator3 = make_ip_part_comparator(any_value<T>(), T()),
  UnaryPredicate<T>&& ip_part_comparator4 = make_ip_part_comparator(any_value<T>(), T()))
{
  IpFilter<T> ip_filter;
  reinit_ip_filter(
    ip_filter,
    std::forward<UnaryPredicate<T>>(ip_part_comparator1),
    std::forward<UnaryPredicate<T>>(ip_part_comparator2),
    std::forward<UnaryPredicate<T>>(ip_part_comparator3),
    std::forward<UnaryPredicate<T>>(ip_part_comparator4));
  return ip_filter;
}

/*!
\class
\brief Шаблонный класс функтор, предоставляющий интерфейс унарного предиката.
На его вход подается контейнер со сравнимым типом параметра шаблона.
operator() выполняет операции сравнения к каждому октету ip адреса. Результаты
сравнений объединяются конъюкцией. Операции сранения и значения для сравнения задаются
при конструировании экземпляра класса.
*/
template<typename T>
class IpFilter {

  friend void reinit_ip_filter<T>(
    IpFilter<T>&,
    UnaryPredicate<T>&&,
    UnaryPredicate<T>&&,
    UnaryPredicate<T>&&,
    UnaryPredicate<T>&&);

  friend IpFilter<T> make_ip_filter<T>(
    UnaryPredicate<T>&&,
    UnaryPredicate<T>&&,
    UnaryPredicate<T>&&,
    UnaryPredicate<T>&&);

public:

  /*!
  \brief Применение операций сравнения к 4 частям ip адреса из входного контейнера
  и инициализированным эталонным значениям с последующей конъюнкцией результата.
  \param[in] ip Контейнер, содержащий части ip адреса.
  \return bool Результат сравнения. 
  */
  template<typename Container>
  bool operator()(const Container& ip)
  {
    if (4 != ip.size())
      return false;

    typename Container::const_iterator ip_part = std::cbegin(ip);
    for (auto& ip_part_comparator : ip_part_comparators)
    {
      if (!ip_part_comparator(*ip_part))
        return false;
      ++ip_part;
    }
    return true;
  }

private:

  /*!
  \brief Конструктор объявлен как private, чтобы экземпляр класса могла создать лишь
  дружественная функция конструктор, корректно инициализирующая 4 операции сравнения и
  значения для сравнения.
  */
  IpFilter() {}

  /*!
  \brief Конечная функций вызовов add_ip_part_comparator с parameter pack.
  */
  void add_ip_part_comparator() {}

  /*!
  \brief Добавление в коллекцию операций сравнения ip частей. Хотя эта функция вызывается лишь friend
  функциями конструкторами всегда с 4 аргументами, она раелизована с помощью parameter pack для учебных целей.
  \param[in] ip_part_comparator Унарный предикат для сравнения части ip адреса, созданный с помощью make_ip_part_comparator.
  \param[in] ip_part_comparator_other Унарные предикаты для сравнения остальных частей ip адреса, созданные с помощью make_ip_part_comparator.
  */
  template<typename ... TT>
  void add_ip_part_comparator(UnaryPredicate<T>&& ip_part_comparator, UnaryPredicate<TT>&& ... ip_part_comparator_other)
  {
    ip_part_comparators.emplace_back(ip_part_comparator);
    add_ip_part_comparator(std::forward<UnaryPredicate<TT>>(ip_part_comparator_other)...);
  }

  /*!
  \brief Сброс сохраненных операций сравнеия и значений для сравнения. Необходима для
  реинициализации уже созданного экземпляра класса.
  */
  void clear_comparators()
  {
    ip_part_comparators.clear();
  }

  std::vector<UnaryPredicate<T>> ip_part_comparators;
};