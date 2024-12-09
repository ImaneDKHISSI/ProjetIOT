from django.contrib import admin
from django.urls import path, include
from django.contrib.auth import views


urlpatterns = [
    path('admin/', admin.site.urls),
    path('DHT/', include('DHT.urls')), #on met DHT/ c'est l'URL du projet pour acceder il faut mettre /DHT/test (test c'est la fonction)
    path('', include('DHT.urls')),
]