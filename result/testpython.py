# -*- coding: utf-8 -*-
import formula

a = formula.Formula(type=formula.EQ_SIN)
#a = formula.Formula()
print a
print a.toStarMath()

print "EQ_COS = %d" % formula.EQ_COS