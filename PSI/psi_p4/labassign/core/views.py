from django.shortcuts import render
from django.utils import timezone
from django.contrib.auth import authenticate, login, logout
from django.http import HttpResponse
from django.urls import reverse
from django.shortcuts import redirect
from django.contrib.auth.decorators import login_required
from core.models import LabGroup, Pair, OtherConstraints, Student
from core.forms import PairForm, GroupForm, BreakPairForm
from django.db.models import Q

# view for the home page
def home(request):
    # get the user that is logged in in this moment
    user = request.user
    # create a dictionary that will help us ih the html
    # when showing messages
    context_dict = {}
    # search if the user is the first member of a pair.
    # we will need this to show the name of the pair
    # in the html
    try:
        pairs = Pair.objects.filter(student1_id=user.id).first()
    except pairs.doesNotExist:
        context_dict = {}

    # search if the user is the second member of a pair.
    # we will need this to show the name of the pair
    # in the html
    try:
        pairs2 = Pair.objects.filter(student2_id=user.id,
                                     validated=True).first()
        # puts the pair where the user is in in the dictionary
        context_dict = {'pairs': pairs, 'pairs2': pairs2}
    except pairs.doesNotExist:
        context_dict = {}
    # returns the dictionary to the home page
    return render(request, 'core/home.html', context=context_dict)


# view for the applygroup page
def applygroup(request):
    # get the user that is logged in in this moment
    user = request.user
    # create a dictionary that will help us ih the html
    # when showing messages
    context_dict = {}
    # create a set to send it to the forms.py
    listID = set()
    # variable for the constrains we will need
    cons = OtherConstraints.objects.get(id=1)
    # variable for our time
    dt = timezone.now()
    # we put the variables already created in the dictionary
    context_dict = {'cons': cons, 'dt': dt}
    # if the user is authenticated
    if request.user.is_authenticated:
        # if the user has used the dropdown menu:
        if request.method == 'POST':
            # gets the answer of the user
            form = GroupForm(user, listID, request.POST)
            if form.is_valid():
                # if it is correct it will take the id of the lab
                # the user wants to join to
                labGroupID = form['myLabGroup'].value()
                # it gets the lab from the id to have all the other
                # variables
                lab = LabGroup.objects.get(id=labGroupID)
                if cons.selectGroupStartDate <= dt:
                    pair = Pair.objects.filter((Q(student1=user)
                                                | Q(student2=user)),
                                               Q(validated=True)).first()
                    # if the date is allowed and the user is not in a pair,
                    # the program add him to the lab group if it has not
                    # achieved the maxNumberOfStudents the lab can get
                    # after adding him to the lab, it updates the lab counter
                    if not pair:
                        if lab.counter < lab.maxNumberStudents:
                            Student.objects.filter(id=user.id). \
                                update(labGroup=labGroupID)
                            LabGroup.objects.filter(id=labGroupID). \
                                update(counter=lab.counter + 1)
                    # if the date is allowed but the user IS in a pair,
                    # the program add him to the lab group he asked for
                    # as well as the partner he has, but if
                    # the lab group has not achieved the maxNumberOfStudents
                    # the lab can get
                    # after adding him and his partner to the lab, it updates
                    # the lab counter by 2
                    else:
                        if lab.counter <= lab.maxNumberStudents - 1:
                            Student.objects.filter(id=pair.student1.id). \
                                update(labGroup=labGroupID)
                            Student.objects.filter(id=pair.student2.id). \
                                update(labGroup=labGroupID)
                            LabGroup.objects.filter(id=labGroupID). \
                                update(counter=lab.counter + 2)
        # show the dropdown menu
        else:
            form = GroupForm(user, listID)
            context_dict["form"] = form
        # return the dict (with the variables we will need and the menu)
        # to the applygroup page
        return render(request, 'core/applygroup.html', context_dict)
    # if the user is not authenticated it will redirect him to the
    # login page
    else:
        return render(request, 'core/login.html')


# view for the breakpair page
def breakpair(request):
    user = request.user
    listID = set()
    context_dict = {}
    # if the user is authenticated
    if user.is_authenticated:
        # if the user has used the dropdown menu:
        if request.method == 'POST':
            # gets the answer of the user
            form = BreakPairForm(user, listID, request.POST)
            if form.is_valid():
                # if it is correct it will take
                # the id of the pair
                pair = form['myPair'].value()
                # it gets the logged user
                stud = Student.objects.get(id=request.user.id)
                # get the student2 id
                stud2 = None
                if Pair.objects.filter(id=pair, student1=stud).exists():
                    stud2 = Pair.objects.get(id=pair, student1=stud).student2
                elif Pair.objects.filter(id=pair, student2=stud).exists():
                    stud2 = Pair.objects.get(id=pair, student2=stud).student1
                if not stud2:
                    context_dict = {'error': stud2}
                    return render(request, 'core/breakpair.html', context=context_dict)
                # the logged user is the one that ask
                # breaking the pair
                # if Pair.objects.filter(id=pair, student1=stud, student2=stud2).exists() or \
                #        Pair.objects.filter(id=pair, student1=stud2, student2=stud).exists():

                # if the user and his pair are not already in a group
                if Student.objects.filter(id=stud.id, labGroup=None).exists() and \
                        Student.objects.filter(id=stud2.id, labGroup=None).exists():
                    # if the pair is not validated
                    if Pair.objects.filter(id=pair, validated=False).exists():
                        # deletes the pair
                        Pair.objects.filter(id=pair, validated=False).delete()
                    # if the pair is validated
                    elif Pair.objects.filter(id=pair, validated=True).exists():
                        # if there is not a request for breaking the pair yet
                        if Pair.objects.filter(id=pair, validated=True,
                                               studentBreakRequest=None).exists():
                            p = Pair.objects.get(id=pair, validated=True,
                                                 studentBreakRequest=None)
                            p.studentBreakRequest = stud
                            p.save()
                        # if there is a request for breaking the pair already
                        else:
                            # deletes the pair
                            Pair.objects.filter(id=pair, validated=True).delete()
        # show the dropdown menu
        else:
            context_dict = {'error': user}
            form = BreakPairForm(user, listID)
            context_dict["form"] = form
        # return the dict (with the variables we will need and the menu)
        # to the breakpair page
        return render(request, 'core/breakpair.html', context=context_dict)
    # if the user is not authenticated it will redirect him to the
    # login page
    else:
        return render(request, 'core/login.html')


# view for the applypair page
def applypair(request):
    # get the user that is logged in in this moment
    user = request.user.id
    selectedPair = False
    # create a dictionary that will help us ih the html
    # when showing messages
    context_dict = {}
    listID = set()
    # if the user is authenticated
    if request.user.is_authenticated:
        # if the user has used the dropdown menu:
        if request.method == 'POST':
            # gets the answer of the user
            form = PairForm(user, listID, request.POST)
            if form.is_valid():
                # if it is correct it will take the id of the
                # new pair that is created
                secondMemberID = form['secondMemberGroup'].value()
                # it gets the logged user
                stud = Student.objects.get(id=request.user.id)
                # it gets the partner the logged user selected in the menu
                stud2 = Student.objects.get(id=secondMemberID)
                if Pair.objects.filter(student1=stud2, student2=stud).exists():
                    # if the pair of the student1 = stud2 and
                    # student2 = stud exists,
                    # it means it was already created so it set the validated
                    # field to True
                    pair = Pair.objects.get(student1=stud2, student2=stud)
                    pair.validated = True
                    # if the stud2 has a labgroup, it will be the same as the
                    # stud.
                    stud.labGroup = stud2.labGroup
                    stud.save()
                    pair.save()
                else:
                    # if the pair of the student1 = stud2 and
                    # student2 = stud does not exists,
                    # it has not been created so it creates it
                    # if and only if stud has not already
                    # a pair.
                    if not Pair.objects.filter(student1=stud).exists():
                        Pair.objects.create(student1=stud,
                                            student2=stud2,
                                            validated=False)
                    else:
                        # if the selected pair has already a partner
                        selectedPair = True
                        context_dict = {'selectedPair': selectedPair}
        # show the dropdown menu
        else:
            form = PairForm(user, listID)
            context_dict["form"] = form
        # return the dict (with the variables we will need and the menu)
        # to the applypair page
        return render(request, 'core/applypair.html', context_dict)
    # if the user is not authenticated it will redirect him to the
    # login page
    else:
        return render(request, 'core/login.html')


# view for the convalidation page
def convalidation(request):
    # get the user that is logged in in this moment
    user = request.user
    # puts in variables the constrains needed
    otherConstrains = OtherConstraints.objects.get(id=1)
    pairs1 = Pair.objects.filter(student1_id=user.id).first()
    pairs2 = Pair.objects.filter(student2_id=user.id, validated=True).first()
    # create a dictionary that will help us ih the html
    # when showing messages
    context_dict = {'user': user,
                    'otherConstrains': otherConstrains,
                    'pairs1': pairs1,
                    'pairs2': pairs2}
    # if the user is authenticated
    if request.user.is_authenticated:
        # if the user you are logged with has not a pair
        # and the grades of the last year are grater than the ones
        # specified in the other constrains, it updates the user convalidation
        # to True
        if not Pair.objects.filter(student1=user).exists() or not \
                Pair.objects.filter(student2=user, validated=True).exists():
            if user.gradeLabLastYear >= otherConstrains.minGradeLabConv \
                    and user.gradeTheoryLastYear >= \
                    otherConstrains.minGradeTheoryConv:
                Student.objects.filter(id=user.id). \
                    update(convalidationGranted=True, labGroup="")
                # return the dict (with the variables
                # we will need and the menu)
                # to the convalidation page when the convalidation is done
                return render(request,
                              'core/convalidation.html', context=context_dict)
            else:
                # return the dict (with the variables
                # we will need and the menu)
                # to the convalidation page when the convalidation is not done
                return render(request,
                              'core/convalidation.html', context=context_dict)
    # if the user is not authenticated it will redirect him to the
    # login page
    else:
        return render(request, 'core/login.html')


# view for the login page
def user_login(request):
    if request.method == 'POST':
        username = request.POST.get('username')
        password = request.POST.get('password')
        user = authenticate(username=username, password=password)
        if user:
            if user.is_active:
                login(request, user)
                return redirect(reverse('core:home'))
            else:
                return HttpResponse("Your account is disabled.")
        else:
            print("Invalid login details: {0}, {1}".format(username, password))
            return HttpResponse("Invalid login details supplied.")
    # if the user is not authenticated it will redirect him to the
    # login page
    else:
        return render(request, 'core/login.html')


# view for the logout
@login_required
# if the user is authenticated
def user_logout(request):
    logout(request)
    return redirect(reverse('core:home'))
