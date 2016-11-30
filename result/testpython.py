# -*- coding: utf-8 -*-
import formula

a = formula.Formula(type=formula.EQ_SIN)
#a = formula.Formula()
print a
print a.toStarMath()

b=formula.sin('%ialpha')
print b.toStarMath()
c=formula.cos(b) / (formula.symbol("a_1")+"c_2"-5.1)
c /= b + 3.5

print c.toStarMath()

d = formula.symbol("a") / (formula.number(2)/"b")
print d.toStarMath()

print "EQ_COS = %d" % formula.EQ_COS