When doing flake8 *.py i get:
populate.py:16:1: E402 module level import not at top of file
populate.py:17:1: E402 module level import not at top of file
populate.py:20:1: E402 module level import not at top of file
populate.py:21:1: E402 module level import not at top of file
populate.py:22:1: E402 module level import not at top of file
populate.py:23:1: E402 module level import not at top of file

This is because we need to put 

import os
import django

os.environ.setdefault('DJANGO_SETTINGS_MODULE',
                      'labassign.settings')
django.setup()

for doing the tests
