# Generated by Django 2.2.5 on 2020-11-23 22:06

from django.conf import settings
import django.contrib.auth.models
import django.contrib.auth.validators
from django.db import migrations, models
import django.db.models.deletion
import django.utils.timezone


class Migration(migrations.Migration):

    initial = True

    dependencies = [
        ('auth', '0011_update_proxy_permissions'),
    ]

    operations = [
        migrations.CreateModel(
            name='Student',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('password', models.CharField(max_length=128, verbose_name='password')),
                ('last_login', models.DateTimeField(blank=True, null=True, verbose_name='last login')),
                ('is_superuser', models.BooleanField(default=False, help_text='Designates that this user has all permissions without explicitly assigning them.', verbose_name='superuser status')),
                ('username', models.CharField(error_messages={'unique': 'A user with that username already exists.'}, help_text='Required. 150 characters or fewer. Letters, digits and @/./+/-/_ only.', max_length=150, unique=True, validators=[django.contrib.auth.validators.UnicodeUsernameValidator()], verbose_name='username')),
                ('first_name', models.CharField(blank=True, max_length=30, verbose_name='first name')),
                ('last_name', models.CharField(blank=True, max_length=150, verbose_name='last name')),
                ('email', models.EmailField(blank=True, max_length=254, verbose_name='email address')),
                ('is_staff', models.BooleanField(default=False, help_text='Designates whether the user can log into this admin site.', verbose_name='staff status')),
                ('is_active', models.BooleanField(default=True, help_text='Designates whether this user should be treated as active. Unselect this instead of deleting accounts.', verbose_name='active')),
                ('date_joined', models.DateTimeField(default=django.utils.timezone.now, verbose_name='date joined')),
                ('gradeTheoryLastYear', models.FloatField(default=0)),
                ('gradeLabLastYear', models.FloatField(default=0)),
                ('convalidationGranted', models.BooleanField(default=False)),
                ('groups', models.ManyToManyField(blank=True, help_text='The groups this user belongs to. A user will get all permissions granted to each of their groups.', related_name='user_set', related_query_name='user', to='auth.Group', verbose_name='groups')),
            ],
            options={
                'ordering': ('last_name', 'first_name'),
            },
            managers=[
                ('objects', django.contrib.auth.models.UserManager()),
            ],
        ),
        migrations.CreateModel(
            name='OtherConstraints',
            fields=[
                ('id', models.IntegerField(default='0', primary_key=True, serialize=False)),
                ('selectGroupStartDate', models.DateTimeField()),
                ('minGradeTheoryConv', models.FloatField(default=0.0)),
                ('minGradeLabConv', models.FloatField(default=0.0)),
            ],
        ),
        migrations.CreateModel(
            name='Teacher',
            fields=[
                ('id', models.IntegerField(default='0', primary_key=True, serialize=False)),
                ('first_name', models.CharField(max_length=30)),
                ('last_name', models.CharField(max_length=30)),
            ],
            options={
                'ordering': ('last_name', 'first_name'),
            },
        ),
        migrations.CreateModel(
            name='TheoryGroup',
            fields=[
                ('id', models.IntegerField(default='0', primary_key=True, serialize=False)),
                ('groupName', models.CharField(max_length=1024)),
                ('language', models.CharField(max_length=100)),
            ],
            options={
                'ordering': ('groupName',),
            },
        ),
        migrations.CreateModel(
            name='Pair',
            fields=[
                ('id', models.IntegerField(default='0', primary_key=True, serialize=False)),
                ('validated', models.BooleanField(default=False)),
                ('student1', models.ForeignKey(null=True, on_delete=django.db.models.deletion.SET_NULL, related_name='student1', to=settings.AUTH_USER_MODEL)),
                ('student2', models.ForeignKey(null=True, on_delete=django.db.models.deletion.SET_NULL, related_name='student2', to=settings.AUTH_USER_MODEL)),
                ('studentBreakRequest', models.ForeignKey(null=True, on_delete=django.db.models.deletion.CASCADE, related_name='break_req', to=settings.AUTH_USER_MODEL)),
            ],
            options={
                'ordering': ('student1__id', 'student2__id'),
            },
        ),
        migrations.CreateModel(
            name='LabGroup',
            fields=[
                ('id', models.IntegerField(default='0', primary_key=True, serialize=False)),
                ('groupName', models.CharField(max_length=50)),
                ('language', models.CharField(max_length=50)),
                ('schedule', models.CharField(max_length=50)),
                ('maxNumberStudents', models.IntegerField()),
                ('counter', models.IntegerField(default=0)),
                ('teacher', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='core.Teacher')),
            ],
            options={
                'ordering': ('groupName',),
            },
        ),
        migrations.CreateModel(
            name='GroupConstraints',
            fields=[
                ('id', models.IntegerField(default='0', primary_key=True, serialize=False)),
                ('labGroup', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='core.LabGroup')),
                ('theoryGroup', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='core.TheoryGroup')),
            ],
            options={
                'ordering': ('labGroup', 'theoryGroup'),
            },
        ),
        migrations.AddField(
            model_name='student',
            name='labGroup',
            field=models.ForeignKey(null=True, on_delete=django.db.models.deletion.DO_NOTHING, to='core.LabGroup'),
        ),
        migrations.AddField(
            model_name='student',
            name='theoryGroup',
            field=models.ForeignKey(null=True, on_delete=django.db.models.deletion.DO_NOTHING, to='core.TheoryGroup'),
        ),
        migrations.AddField(
            model_name='student',
            name='user_permissions',
            field=models.ManyToManyField(blank=True, help_text='Specific permissions for this user.', related_name='user_set', related_query_name='user', to='auth.Permission', verbose_name='user permissions'),
        ),
    ]