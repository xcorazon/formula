# -*- coding: utf-8 -*-
import formula

a = formula.Formula(type=formula.EQ_SIN)
#a = formula.Formula()
print a
print a.toStarMath()

b=formula.sin('%ialpha')
print b.toStarMath()
c=formula.cos(b)
print c.toStarMath()

print "EQ_COS = %d" % formula.EQ_COS