from django.contrib import admin
from core.models import Student, Teacher, LabGroup, \
    GroupConstraints, Pair, OtherConstraints, TheoryGroup

admin.site.register(Student)
admin.site.register(Teacher)
admin.site.register(LabGroup)
admin.site.register(GroupConstraints)
admin.site.register(Pair)
admin.site.register(OtherConstraints)
admin.site.register(TheoryGroup)
