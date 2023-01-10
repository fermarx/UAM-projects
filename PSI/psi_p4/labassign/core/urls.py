from django.urls import path
from core import views

app_name = 'core'

urlpatterns = [
    path('', views.home, name='home'),
    path('login/', views.user_login, name='login'),
    path('logout/', views.user_logout, name='logout'),
    path('convalidation/', views.convalidation, name='convalidation'),
    path('applypair/', views.applypair, name='applypair'),
    path('breakpair/', views.breakpair, name='breakpair'),
    path('applygroup/', views.applygroup, name='applygroup'),
]
