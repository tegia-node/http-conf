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

	make libLISTENER
	make libCONNECTION
#	make libA2Session


# =============================================================================================================
#
# BUILD ACTORS
#
# =============================================================================================================

libLISTENER:
	g++ -c -o FCgiIO.o              ../src/actors/LISTENER/common/FCgiIO.cpp    $(Flag) -fPIC
	g++ -c -o LISTENER.o            ../src/actors/LISTENER/LISTENER.cpp         $(Flag) -fPIC
	g++ -shared -fPIC -o libLISTENER.so             \
	     FCgiIO.o                \
	     LISTENER.o -L/usr/lib -lcgicc -lfcgi -lfcgi++ -lnlohmann_json_schema_validator $(Flag)


libCONNECTION:
	g++ -c -o FCgiIO.o              ../src/actors/LISTENER/common/FCgiIO.cpp    $(Flag) -fPIC
	g++ -c -o CONNECTION.o          ../src/actors/CONNECTION/CONNECTION.cpp -I../vendors/cpp-jwt/include/ $(Flag) -fPIC
	g++ -shared -fPIC -o libCONNECTION.so     \
		FCgiIO.o \
	    CONNECTION.o -L/usr/lib -lcgicc -lfcgi -lfcgi++ -lnlohmann_json_schema_validator $(Flag)




libA2Sessions:
	g++ -c -o A2Sessions.o      ../actors/A2Sessions/A2Sessions.cpp -I../vendors/cpp-jwt/include/ $(Flag) -fPIC
	g++ -shared -fPIC -o libA2Sessions.so     \
	     A2Sessions.o -lstdc++fs $(Flag)		 
