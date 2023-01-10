import django
import os

from core.models import (OtherConstraints, Pair, Student)
from django.utils import timezone
from datetime import timedelta
from labassign.settings import BASE_DIR
from django.test import TestCase
django.setup()
pathToProject = BASE_DIR
os.environ.setdefault('DJANGO_SETTINGS_MODULE',
                      'labassign.settings')


class QueryTestUsers(TestCase):
    allow_database_queries = True

    def test_create_student1000(self):
        try:
            user_1000 = Student.objects.filter(id=1000).get()
        except:
            user_1000 = Student.objects.create(id=1000,
                                               first_name="Pepito",
                                               last_name="Grillo",
                                               username=1234)
        user_1000.save()
        self.assertEqual(user_1000, Student.objects.get(id=1000))

    def test_create_student1001(self):
        try:
            user_1001 = Student.objects.filter(id=1001).get()
        except:
            user_1001 = Student.objects.create(id=1001,
                                               first_name="Juan",
                                               last_name="Cuesta",
                                               username=4321)
        user_1001.save()
        self.assertEqual(user_1001, Student.objects.get(id=1001))

    def test_create_pair(self):
        try:
            user_1000 = Student.objects.filter(id=1000).get()
        except:
            user_1000 = Student.objects.create(id=1000,
                                               first_name="Pepito",
                                               last_name="Grillo",
                                               username=1234)
        user_1000.save()

        try:
            user_1001 = Student.objects.get(id=1001)
        except:
            user_1001 = Student.objects.create(id=1001,
                                               first_name="Juan",
                                               last_name="Cuesta",
                                               username=4321)
        user_1001.save()

        p = Pair.objects.create(id=0, student1=Student.objects.get(id=1000),
                                student2=Student.objects.get(id=1001))
        print(p)
        self.assertEqual(p, Pair.objects.get(id=0,
                                             student1=Student
                                             .objects.get(id=1000),
                                             student2=Student
                                             .objects.get(id=1001)))
        Pair.objects.filter(id=0, student1=Student.objects.get(id=1000),
                            student2=Student.objects.get(id=1001)) \
            .update(validated=True)


class QueryTestOtherConstraints(TestCase):
    allow_database_queries = True

    def test_OtherConstraints(self):
        dt = timezone.now()
        td = timedelta(days=1)
        my_date = dt + td
        otherConstraints = OtherConstraints. \
            objects.create(id=1, selectGroupStartDate=my_date,
                           minGradeTheoryConv=7.0,
                           minGradeLabConv=3.0)
        if otherConstraints.selectGroupStartDate < dt:
            print(str(otherConstraints.selectGroupStartDate)
                  + " is in the past")
        else:
            print(str(otherConstraints.selectGroupStartDate)
                  + " is in the future")
