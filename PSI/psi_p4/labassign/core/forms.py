from django import forms
from core.models import GroupConstraints, Pair, Student
from django.contrib.auth.models import User
from django.db.models import Q


class BreakPairForm(forms.Form):
    # creation of the pulldown menu
    myPair = forms.ModelChoiceField(
        queryset=None, label="Select the pair to be broken")

    def __init__(self, user, listID, *args, **kwargs):
        super(forms.Form, self).__init__(*args, **kwargs)
        # filter all the Pairs you are interested
        # all the ones where the user is in
        pair = Pair.objects.filter(student1=user).values_list('student1', flat=True)
        # convert to a list
        pair = [entry for entry in pair]
        # put it in to a list
        listID = listID.union(set(pair))

        # filter all the Pairs you are interested
        # all the ones where the user is in
        pair = Pair.objects.filter(student2=user).values_list('student2', flat=True)
        # convert to a list
        pair = [entry for entry in pair]
        # put it in to a list
        listID = listID.union(set(pair))

        # get all the pairs that exist in the database
        # even they are not validated
        self.fields['myPair'].queryset = Pair.objects.all().filter(Q(student1__in=listID) | Q(student2__in=listID))


# class from for the lab group formation
class GroupForm(forms.Form):
    # creation of the pulldown menu
    myLabGroup = forms.ModelChoiceField(
        queryset=None, label="Select the group you want to join to")

    def __init__(self, user, listID, *args, **kwargs):
        super(forms.Form, self).__init__(*args, **kwargs)
        # filter all the groups you are interested in
        # you want the ones that have the group constraints
        # theory group the same
        groupsCant = GroupConstraints.\
            objects.filter(theoryGroup=user.theoryGroup)\
            .values_list('theoryGroup', flat=True)
        # convert to a list
        groupsCant = [entry for entry in groupsCant]
        # put it in to a list
        listID = listID.union(set(groupsCant))
        # put in the pulldown menu the lab groups you got in
        # the prevoious cases
        self.fields['myLabGroup'].queryset = GroupConstraints\
            .objects.filter(theoryGroup__in=listID)


# class from for the pair formation
class PairForm(forms.Form):
    # creation of the pulldown menu
    secondMemberGroup = forms.ModelChoiceField(queryset=None,
                                               label="Select the second"
                                                     " member of the pair")

    def __init__(self, user, listID, *args, **kwargs):
        super(forms.Form, self).__init__(*args, **kwargs)
        # filter all the groups you are not interested in
        # you don not want yourself to be in a pair with
        listID.add(user)

        # you do not want someone that already have a pair
        pairValidated1 = Pair.objects.filter(validated=True).\
            values_list('student1', flat=True)
        # convert to a list
        pairValidated1 = [entry for entry in pairValidated1]
        # put it in to a list
        listID = listID.union(set(pairValidated1))

        # you do not want someone that already have a pair
        pairValidated2 = Pair.objects.filter(validated=True).\
            values_list('student2', flat=True)
        # convert to a list
        pairValidated2 = [entry for entry in pairValidated2]
        # put it in to a list
        listID = listID.union(set(pairValidated2))

        # eclude from the pulldown menu the students you got in
        # the prevoious cases
        self.fields['secondMemberGroup'].queryset = Student.\
            objects.all().exclude(id__in=listID)


class UserForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput())

    class Meta:
        model = User
        fields = ('username', 'password')
