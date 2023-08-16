include ../../../Makefile.variable

# =============================================================================================================
#
# BUILD
#
# =============================================================================================================

http:
	@echo " "
	@echo "------------------------------------------------------------"
	@echo "HTTP CONFIGURATION: \033[32;1m BUILD \033[0m"
	@echo "------------------------------------------------------------"
	@echo " "

	make libA2FCGI
	make libA2Sessions
	make libA2Session


# =============================================================================================================
#
# INSTALL SOURCE
#
# =============================================================================================================

install:
	@echo " "
	@echo "------------------------------------------------------------"
	@echo "HTTP CONFIGURATION: \033[32;1m CONFIGURE SOURCE \033[0m"
	@echo "------------------------------------------------------------"
	@echo " "

	sh ./install.sh
	make http

	@echo " "
	@echo "HTTP CONFIGURATION: \033[32;1m INSTALL SUCCESS \033[0m"
	@echo " "


# =============================================================================================================
#
# CREATE DEB PACKAGE
#
# =============================================================================================================

http-deb:
	make http

	@echo " "
	@echo "------------------------------------------------------------"
	@echo "HTTP CONFIGURATION: \033[32;1m CREATE DEB PACKAGE \033[0m"
	@echo "------------------------------------------------------------"
	@echo " "

	cp -r *.so ../deb/tegia-http/etc/tegia/http/bin/
	cp http.config.json ../deb/tegia-http/etc/tegia/http/bin/http.config.json
	cp ../db/install.sql ../deb/tegia-http/etc/tegia/http/db/install.sql
	cp ../db/uninstall.sql ../deb/tegia-http/etc/tegia/http/db/uninstall.sql
#   Собираем deb-пакет
	@echo "\033[32mmake deb-package\033[0m"
	cd ../deb/ && fakeroot dpkg-deb --build tegia-http
	cp ../deb/tegia-http.deb ../../../debs/tegia-http.deb




# =============================================================================================================
#
# BUILD ACTORS
#
# =============================================================================================================

libA2FCGI:
	g++ -c -o FCgiIO.o              ../actors/A2FCGI/FCgiIO.cpp $(Flag)	 -fPIC
	g++ -c -o A2FCGI.o              ../actors/A2FCGI/A2FCGI.cpp $(Flag) -fPIC
	g++ -shared -fPIC -o libA2FCGI.so             \
	     FCgiIO.o                \
	     A2FCGI.o -L/usr/lib -lcgicc -lfcgi -lfcgi++ -lnlohmann_json_schema_validator $(Flag)


libA2Session:
	g++ -c -o FCgiIO.o              ../actors/A2FCGI/FCgiIO.cpp $(Flag)	 -fPIC
	g++ -c -o A2Session.o      ../actors/A2Session/A2Session.cpp -I../vendors/cpp-jwt/include/ $(Flag) -fPIC
	g++ -shared -fPIC -o libA2Session.so     \
		FCgiIO.o \
	    A2Session.o -L/usr/lib -lcgicc -lfcgi -lfcgi++ -lnlohmann_json_schema_validator $(Flag)

libA2Sessions:
	g++ -c -o A2Sessions.o      ../actors/A2Sessions/A2Sessions.cpp -I../vendors/cpp-jwt/include/ $(Flag) -fPIC
	g++ -shared -fPIC -o libA2Sessions.so     \
	     A2Sessions.o -lstdc++fs $(Flag)		 
