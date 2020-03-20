#ifndef ELEC4_UTIL_H
#define ELEC4_UTIL_H


template<typename tpl_t>
class Clamp {
 private:
  tpl_t min_;
  tpl_t max_;

 public:
  explicit Clamp(tpl_t min, tpl_t max): min_{min}, max_{max}{}
  int operator()(tpl_t value){
    return (value <= min_) ? min_ : ((value >= max_) ? max_ : value);
  }
};
#endif // ELEC4_UTIL_H
