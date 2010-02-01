#include <ruby.h>
#include "clips/clips.h"
#include "rbclips.h"
#include "rbgeneric.h"

/**
 * Transfer ruby ID to string representation that is valid in CLIPS.
 * :symbol => SYMBOL, :any => ?VARIABLE 
 */
const char const * rb_generic_clipstype_str(ID id)
{
# define TRANSFER(source, target) if(id == cl_vIds.source) return target
  TRANSFER(symbol,            "SYMBOL");
  TRANSFER(string,            "STRING");
  TRANSFER(lexeme,            "LEXEME");
  TRANSFER(integer,           "INTEGER");
  TRANSFER(float_,            "FLOAT");
  TRANSFER(number,            "NUMBER");
  TRANSFER(instance_name,     "INSTANCE-NAME");
  TRANSFER(instance_address,  "INSTANCE-ADDRESS");
  TRANSFER(instance,          "INSTANCE");
  TRANSFER(external_address,  "EXTERNAL-ADDRESS");
  TRANSFER(fact_address,      "FACT-ADDRESS");
  TRANSFER(any,               "?VARIABLE");
# undef TRANSFER

  return "";
}

/**
 * Check if given value is valid CLIPS symbol (don't have spaces)
 */
int cl_generic_check_clips_symbol(VALUE s)
{
  VALUE argv[1];
  argv[0] = rb_str_new2("^[^ \"]+$");

  VALUE regexp = rb_class_new_instance(1, argv, rb_cRegexp);

  VALUE ret = rb_funcall(regexp, cl_vIds.eqq, 1, s);

  if(TYPE(ret) == T_TRUE) return true;
  
  return false;
}

/**
 * Convert value on index from multifield DATA_OBJECT to corresponding
 * object in ruby world and return it
 */
VALUE cl_generic_convert_dataobject_mf(void *mf, int index)
{
  void *value = GetMFValue(mf, index);

  switch(GetMFType(mf, index))
  {
    case INTEGER:
      return INT2NUM( ValueToInteger(value) );

    case SYMBOL:
      return ID2SYM( rb_intern( ValueToString(value) ) );

    case STRING:
      return rb_str_new_cstr( ValueToString(value) );

    case FLOAT:
      return rb_float_new( ValueToDouble(value) );

    default:
      break;
  }

  return Qfalse;
}
