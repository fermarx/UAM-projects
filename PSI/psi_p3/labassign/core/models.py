from django.contrib.auth.models import AbstractUser
from django.db import models


class Teacher(models.Model):
    id = models.IntegerField(primary_key=True, default='0')
    first_name = models.CharField(max_length=30)
    last_name = models.CharField(max_length=30)

    def __str__(self):
        return self.last_name + ", " + self.first_name

    class Meta:
        ordering = ('last_name', 'first_name',)

    def save(self, *args, **kwargs):
        super(Teacher, self).save(*args, **kwargs)


class TheoryGroup(models.Model):
    id = models.IntegerField(primary_key=True, default='0')
    groupName = models.CharField(max_length=1024)
    language = models.CharField(max_length=100)

    def __str__(self):
        return ("Group name: " + self.groupName + "\n" +
                "Language: " + self.language)

    class Meta:
        ordering = ('groupName',)

    def save(self, *args, **kwargs):
        super(TheoryGroup, self).save(*args, **kwargs)


class LabGroup(models.Model):
    id = models.IntegerField(primary_key=True, default='0')
    teacher = models.ForeignKey(Teacher, on_delete=models.CASCADE)
    groupName = models.CharField(max_length=50)
    language = models.CharField(max_length=50)
    schedule = models.CharField(max_length=50)
    maxNumberStudents = models.IntegerField()
    counter = models.IntegerField(default=0)

    def __str__(self):
        return ("Teacher: " + self.teacher.first_name
                + self.teacher.last_name + "\n" +
                'Group name: ' + self.groupName + '\n' +
                'Schedule: ' + self.schedule + '\n' +
                'Language: ' + self.language)

    class Meta:
        ordering = ('groupName',)

    def save(self, *args, **kwargs):
        super(LabGroup, self).save(*args, **kwargs)


class GroupConstraints(models.Model):
    id = models.IntegerField(primary_key=True, default='0')
    theoryGroup = models.ForeignKey(TheoryGroup, on_delete=models.CASCADE)
    labGroup = models.ForeignKey(LabGroup, on_delete=models.CASCADE)

    def __str__(self):
        return ("Theory group: " + self.theoryGroup.groupName + "\n" +
                "Lab group: " + self.labGroup.groupName)

    class Meta:
        ordering = ('labGroup', 'theoryGroup',)

    def save(self, *args, **kwargs):
        super(GroupConstraints, self).save(*args, **kwargs)


class Student(AbstractUser):
    labGroup = models.ForeignKey(LabGroup,
                                 on_delete=models.DO_NOTHING, null=True)
    theoryGroup = models.ForeignKey(TheoryGroup,
                                    on_delete=models.DO_NOTHING, null=True)
    gradeTheoryLastYear = models.FloatField(default=0)
    gradeLabLastYear = models.FloatField(default=0)
    convalidationGranted = models.BooleanField(default=False)

    def __str__(self):
        return self.last_name + ", " + self.first_name

    class Meta:
        ordering = ('last_name', 'first_name',)

    def save(self, *args, **kwargs):
        super(Student, self).save(*args, **kwargs)


class Pair(models.Model):
    id = models.IntegerField(primary_key=True, default='0')
    student1 = models.ForeignKey(Student,
                                 on_delete=models.SET_NULL,
                                 related_name='student1', null=True)
    student2 = models.ForeignKey(Student,
                                 on_delete=models.SET_NULL,
                                 related_name='student2', null=True)
    validated = models.BooleanField(default=False)
    studentBreakRequest = models.ForeignKey(Student,
                                            on_delete=models.CASCADE,
                                            related_name='break_req',
                                            null=True)

    def __str__(self):
        return ("Pair: " + str(self.id) + "\n" +
                "Formed by: " + self.student1.first_name +
                ", " + self.student1.last_name + " and "
                + self.student2.first_name + ", " + self.student2.last_name)

    class Meta:
        ordering = ('student1__id', 'student2__id')

    def save(self, *args, **kwargs):
        super(Pair, self).save(*args, **kwargs)


class OtherConstraints(models.Model):
    id = models.IntegerField(primary_key=True, default='0')
    selectGroupStartDate = models.DateTimeField()
    minGradeTheoryConv = models.FloatField(default=0.0)
    minGradeLabConv = models.FloatField(default=0.0)

    def __str__(self):
        return ("Choose pair after: " + str(self.selectGroupStartDate) + "\n" +
                "Min theory grade: " + str(self.minGradeTheoryConv) + "\n " +
                "Min lab grade: " + str(self.minGradeLabConv))

    def save(self, *args, **kwargs):
        super(OtherConstraints, self).save(*args, **kwargs)
