# Populate database
# This file has to be placed within the
# core/management/commands directory in your project.
# If that directory doesn't exist, create it.
# The name of the script is the name of the custom command,
# that is, populate.py.
#
# execute python manage.py  populate
import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE',
                      'labassign.settings')
django.setup()

import csv
from core.models import Teacher, LabGroup,\
    TheoryGroup, GroupConstraints, Pair,\
    OtherConstraints, Student
from collections import OrderedDict
from django.core.management.base import BaseCommand
from datetime import timedelta
from django.utils import timezone


# The name of this class is not optional must be Command
# otherwise manage.py will not process it properly
#
# Teachers, groups and constraints
# will be hardcoded in this file.
# Students will be read from a cvs file
# last year grade will be obtained from another cvs file


class Command(BaseCommand):
    # helps and arguments shown when command python manage.py help populate
    # is executed.
    help = """populate database
           """

    def add_arguments(self, parser):
        parser.add_argument('model', type=str, help="""
        model to  update:
        all -> all models
        teacher
        labgroup
        theorygroup
        groupconstraints
        otherconstrains
        student (require csv file)
        studentgrade (require different csv file,
        update only existing students)
        pair
        """)
        parser.add_argument('studentinfo', type=str, help="""CSV file with student information
        header= NIE, DNI, Apellidos, Nombre, Teoría
        if NIE or DNI == 0 skip this entry and print a warning""")
        parser.add_argument('studentinfolastyear', type=str, help="""CSV file with student information
        header= NIE,DNI,Apellidos,Nombre,Teoría, grade lab, grade the
        if NIE or DNI == 0 skip this entry and print a warning""")

    # handle is another compulsory name, do not change it"
    def handle(self, *args, **kwargs):
        model = kwargs['model']
        cvsStudentFile = kwargs['studentinfo']
        cvsStudentFileGrades = kwargs['studentinfolastyear']
        # clean database
        if model == 'all':
            self.cleanDataBase()
        if model == 'teacher' or model == 'all':
            self.teacher()
        if model == 'labgroup' or model == 'all':
            self.labgroup()
        if model == 'theorygroup' or model == 'all':
            self.theorygroup()
        if model == 'groupconstraints' or model == 'all':
            self.groupconstraints()
        if model == 'otherconstrains' or model == 'all':
            self.otherconstrains()
        if model == 'student' or model == 'all':
            self.student(cvsStudentFile)
        if model == 'studentgrade' or model == 'all':
            self.studentgrade(cvsStudentFileGrades)
        if model == 'pair' or model == 'all':
            self.pair()

    def cleanDataBase(self):
        # delete all models stored (clean table)
        # in database
        Teacher.objects.all().delete()
        TheoryGroup.objects.all().delete()
        LabGroup.objects.all().delete()
        GroupConstraints.objects.all().delete()
        Student.objects.all().delete()
        Pair.objects.all().delete()
        OtherConstraints.objects.all().delete()

    def teacher(self):
        """create teachers here"""
        # create dictionary with teacher data
        teacherD = {}
        teacherD[1] = {'id': 1,  # 1261, L 18:00, 1271 X 18-20
                       'first_name': 'No',
                       'last_name': 'Asignado1'}
        teacherD[2] = {'id': 2,  # 1262 X 18-20, 1263/1273 V 17-19
                       'first_name': 'No',
                       'last_name': 'Asignado4'}
        teacherD[3] = {'id': 3,  # 1272 V 17-19, 1291 L 18-20
                       'first_name': 'Julia',
                       'last_name': 'Diaz Garcia'}
        teacherD[4] = {'id': 4,  # 1292/1251V 17:00
                       'first_name': 'Alvaro',
                       'last_name': 'del Val Latorre'}
        teacherD[5] = {'id': 5,  # 1201 X 18:00
                       'first_name': 'Roberto',
                       'last_name': 'Marabini Ruiz'}
        for i in range(1, len(teacherD) + 1):
            teacher = Teacher.objects.get_or_create(
                id=teacherD[i]['id'],
                first_name=teacherD[i]['first_name'],
                last_name=teacherD[i]['last_name'],
            )[0]
            teacher.save()

    def labgroup(self):
        """add labgroups"""
        # create dictionary with lab data
        maxNumberStudents = 23
        labgroupD = {}
        labgroupD[1261] = {'id': 1261,  # 1261, L 18:00, 1271 X 18-20
                           'teacher': 1,
                           'groupName': '1261',
                           'schedule': 'Lunes/Monday 18-20',
                           'language': 'español/Spanish',
                           'maxNumberStudents': maxNumberStudents}
        labgroupD[1262] = {'id': 1262,  # 1261, L 18:00, 1271 X 18-20
                           'teacher': 2,
                           'groupName': '1262',
                           'schedule': 'Miércoles/Wednesday 18-20',
                           'language': 'español/Spanish',
                           'maxNumberStudents': maxNumberStudents}
        labgroupD[1263] = {'id': 1263,  # 1261, L 18:00, 1271 X 18-20
                           'teacher': 2,
                           'groupName': '1263',
                           'schedule': 'Viernes/Friday 17-19',
                           'language': 'español/Spanish',
                           'maxNumberStudents': maxNumberStudents}
        labgroupD[1271] = {'id': 1271,  # 1261, L 18:00, 1271 X 18-20
                           'teacher': 1,
                           'groupName': '1271',
                           'schedule': 'Miércoles/Wednesday 18-20',
                           'language': 'español/Spanish',
                           'maxNumberStudents': maxNumberStudents}
        labgroupD[1272] = {'id': 1272,  # 1261, L 18:00, 1271 X 18-20
                           'teacher': 3,
                           'groupName': '1272',
                           'schedule': 'Viernes/Friday 17-19',
                           'language': 'español/Spanish',
                           'maxNumberStudents': maxNumberStudents}
        labgroupD[1291] = {'id': 1291,  # 1261, L 18:00, 1271 X 18-20
                           'teacher': 3,
                           'groupName': '1291',
                           'schedule': 'Lunes/Monday 18-20',
                           'language': 'inglés/English',
                           'maxNumberStudents': maxNumberStudents}
        labgroupD[1292] = {'id': 1292,
                           'teacher': 4,
                           'groupName': '1292',
                           'schedule': 'Viernes/Friday 17-19',
                           'language': 'inglés/English',
                           'maxNumberStudents': maxNumberStudents}
        labgroupD[1201] = {'id': 1201,
                           'teacher': 5,
                           'groupName': '1201',
                           'schedule': 'Miércoles/Wednesday 18-20',
                           'language': 'español/Spanish',
                           'maxNumberStudents': maxNumberStudents}

        for k in range(1261, 1264):
            labgroup = LabGroup.objects.get_or_create(
                id=labgroupD[k]['id'],
                teacher=Teacher.objects.get(id=labgroupD[k]['teacher']),
                groupName=labgroupD[k]['groupName'],
                language=labgroupD[k]['language'],
                schedule=labgroupD[k]['schedule'],
                maxNumberStudents=labgroupD[k]['maxNumberStudents'],
            )[0]
            labgroup.save()

        for k in range(1271, 1273):
            labgroup = LabGroup.objects.get_or_create(
                id=labgroupD[k]['id'],
                teacher=Teacher.objects.get(id=labgroupD[k]['teacher']),
                groupName=labgroupD[k]['groupName'],
                language=labgroupD[k]['language'],
                schedule=labgroupD[k]['schedule'],
                maxNumberStudents=labgroupD[k]['maxNumberStudents'],
            )[0]
            labgroup.save()

        for k in range(1291, 1293):
            labgroup = LabGroup.objects.get_or_create(
                id=labgroupD[k]['id'],
                teacher=Teacher.objects.get(id=labgroupD[k]['teacher']),
                groupName=labgroupD[k]['groupName'],
                language=labgroupD[k]['language'],
                schedule=labgroupD[k]['schedule'],
                maxNumberStudents=labgroupD[k]['maxNumberStudents'],
            )[0]
            labgroup.save()

        labgroup = LabGroup.objects.get_or_create(
            id=labgroupD[1201]['id'],
            teacher=Teacher.objects.get(id=labgroupD[1201]['teacher']),
            groupName=labgroupD[1201]['groupName'],
            language=labgroupD[1201]['language'],
            schedule=labgroupD[1201]['schedule'],
            maxNumberStudents=labgroupD[1201]['maxNumberStudents'],
        )[0]
        labgroup.save()

    def theorygroup(self):
        """add theorygroups"""
        # create dictionary with theory data
        theorygroupD = {}
        theorygroupD[126] = {'id': 126,
                             'groupName': '126',
                             'language': 'español/Spanish', }
        theorygroupD[127] = {'id': 127,  # 127/120
                             'groupName': '127',
                             'language': 'español/Spanish', }
        theorygroupD[129] = {'id': 129,  # 129/125
                             'groupName': '129',
                             'language': 'inglés/English', }
        theorygroupD[120] = {'id': 120,  # 127/120
                             'groupName': '120',
                             'language': 'español/Spanish', }
        theorygroupD[125] = {'id': 125,  # 129/125
                             'groupName': '125',
                             'language': 'inglés/English', }

        for k in range(125, 128):
            theorygroup = TheoryGroup.objects.get_or_create(
                id=theorygroupD[k]['id'],
                groupName=theorygroupD[k]['groupName'],
                language=theorygroupD[k]['language'],
            )[0]
            theorygroup.save()

        theorygroup = TheoryGroup.objects.get_or_create(
            id=theorygroupD[129]['id'],
            groupName=theorygroupD[129]['groupName'],
            language=theorygroupD[129]['language'],
        )[0]
        theorygroup.save()

        theorygroup = TheoryGroup.objects.get_or_create(
            id=theorygroupD[120]['id'],
            groupName=theorygroupD[120]['groupName'],
            language=theorygroupD[120]['language'],
        )[0]
        theorygroup.save()

    def groupconstraints(self):
        """add group constrints"""
        """ Follows which laboratory groups (4th column
            may be choosen by which theory groups (2nd column)
            theoryGroup: 126, labGroup: 1261
            theoryGroup: 126, labGroup: 1262
            theoryGroup: 126, labGroup: 1263
            theoryGroup: 127, labGroup: 1271
            theoryGroup: 127, labGroup: 1272
            theoryGroup: 120, labGroup: 1201
            theoryGroup: 129, labGroup: 1291
            theoryGroup: 125, labGroup: 1292"""
        # create dictionary with group constraints data
        groupconstraintsD = {}
        groupconstraintsD[1261] = {'theoryGroup': 126,
                                   'labGroup': 1261}  # mañana
        groupconstraintsD[1262] = {'theoryGroup': 126,
                                   'labGroup': 1262}  # mañana
        groupconstraintsD[1263] = {'theoryGroup': 126,
                                   'labGroup': 1263}  # mañana
        # tarde, split ii and doble
        groupconstraintsD[1271] = {'theoryGroup': 127,
                                   'labGroup': 1271}  # tarde - no doble
        groupconstraintsD[1272] = {'theoryGroup': 127,
                                   'labGroup': 1272}  # tarde - no doble
        groupconstraintsD[1201] = {'theoryGroup': 120,
                                   'labGroup': 1201}
        # doble - tarde - español - WEds
        # english
        groupconstraintsD[1291] = {'theoryGroup': 129,
                                   'labGroup': 1291}
        # inglés - ii - tarde Friday
        groupconstraintsD[1292] = {'theoryGroup': 125,
                                   'labGroup': 1292}  # inglés - doble
        # groupconstraintsD[1202] = {'theoryGroup' : 120, 'labGroup': 1202}
        # doble - tarde - 2nd group
        for k in range(1261, 1264):
            groupconstrains = GroupConstraints.objects.get_or_create(
                id=groupconstraintsD[k]['labGroup'],
                theoryGroup=TheoryGroup.objects.get(
                    id=groupconstraintsD[k]['theoryGroup']),
                labGroup=LabGroup.objects.get(
                    id=groupconstraintsD[k]['labGroup'])
            )[0]
            groupconstrains.save()

        for k in range(1271, 1273):
            groupconstrains = GroupConstraints.objects.get_or_create(
                id=groupconstraintsD[k]['labGroup'],
                theoryGroup=TheoryGroup.objects.get(
                    id=groupconstraintsD[k]['theoryGroup']),
                labGroup=LabGroup.objects.get(
                    id=groupconstraintsD[k]['labGroup'])
            )[0]
            groupconstrains.save()

        for k in range(1291, 1293):
            groupconstrains = GroupConstraints.objects.get_or_create(
                id=groupconstraintsD[k]['labGroup'],
                theoryGroup=TheoryGroup.objects.get(
                    id=groupconstraintsD[k]['theoryGroup']),
                labGroup=LabGroup.objects.get(
                    id=groupconstraintsD[k]['labGroup'])
            )[0]
            groupconstrains.save()

        groupconstrains = GroupConstraints.objects.get_or_create(
            id=groupconstraintsD[1201]['labGroup'],
            theoryGroup=TheoryGroup.objects.get(
                id=groupconstraintsD[1201]['theoryGroup']),
            labGroup=LabGroup.objects.get(
                id=groupconstraintsD[1201]['labGroup'])
        )[0]
        groupconstrains.save()

    def pair(self):
        """create a few valid pairs"""
        # NOTE: first student id is 1000, second 1001, etc
        # students are order alphabetically(last_name, first_name)
        """ create two requests
            1000 -> 1100
            1001 -> 1001
            create three verified groups
            1010 - 1110
            1011 - 1111
            1012 - 1112
        """
        pairD = {}
        # Mañana
        pairD[1000] = {'student2': 1100, 'validated': False}
        pairD[1001] = {'student2': 1101, 'validated': False}
        pairD[1010] = {'student2': 1110, 'validated': True}
        pairD[1011] = {'student2': 1111, 'validated': True}
        pairD[1012] = {'student2': 1112, 'validated': True}
        OrderedDict(pairD)
        for k in range(1000, 1002):
            pair = Pair.objects.get_or_create(
                id=(pairD[k]["student2"] - 100),
                student1=Student.objects.get(id=(pairD[k]["student2"] - 100)),
                student2=Student.objects.get(id=pairD[k]["student2"]),
                validated=pairD[k]["validated"],
                studentBreakRequest=None,
            )[0]
            pair.save()

        for k in range(1010, 1013):
            pair = Pair.objects.get_or_create(
                id=(pairD[k]["student2"] - 100),
                student1=Student.objects.get(id=(pairD[k]["student2"] - 100)),
                student2=Student.objects.get(id=pairD[k]["student2"]),
                validated=pairD[k]["validated"],
                studentBreakRequest=None,
            )[0]
            pair.save()

    def otherconstrains(self):
        """create a single object here with staarting dates
        and maximum and minimum convalidation grades"""
        """ Use the following values:
        selectGroupStartDate = now + 1 day,
        minGradeTheoryConv = 3,
        minGradeLabConv = 7
        """
        dt = timezone.now()
        td = timedelta(days=1)
        my_date = dt + td
        otherconstrains = OtherConstraints.objects.get_or_create(
            id=1,
            selectGroupStartDate=my_date,
            minGradeTheoryConv=3.0,
            minGradeLabConv=7.0,
        )[0]
        otherconstrains.save()

    def student(self, csvStudentFile):
        # read csv file
        # structure NIE	DNI	Apellidos	Nombre	group-Teoría
        with open(csvStudentFile, mode='r') as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            line_count = 0
            id_help = 1000
            for row in csv_reader:
                if line_count == 0:
                    line_count += 1
                else:
                    student = Student.objects.get_or_create(
                        id=id_help,
                        username=row[0],
                        last_name=row[2],
                        first_name=row[3],
                        theoryGroup=TheoryGroup.objects.get(id=row[4]),
                        gradeLabLastYear=0.0,
                        gradeTheoryLastYear=0.0
                    )[0]
                    student.set_password(row[1])
                    id_help += 1
                    student.save()
                    line_count += 1

    def studentgrade(self, cvsStudentFileGrades):
        # read csv file
        # structure NIE	DNI	Apellidos	Nombre	group-Teoría
        # grade-practicas	gradeteoria
        with open(cvsStudentFileGrades, mode='r') as csv_file:
            csv_reader = csv.reader(csv_file, delimiter=',')
            line_count = 0
            for row in csv_reader:
                if line_count == 0:
                    line_count += 1
                else:
                    Student.objects.filter(
                        username=row[0]
                    ).update(
                        last_name=row[2],
                        first_name=row[3],
                        theoryGroup=TheoryGroup.objects.get(id=row[4]),
                        convalidationGranted=False,
                        gradeLabLastYear=row[5],
                        gradeTheoryLastYear=row[6])
                    line_count += 1
