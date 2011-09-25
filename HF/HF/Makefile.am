AM_CXXFLAGS=-g

bin_PROGRAMS = HF

HF_SOURCES =    background.h		    background.cpp \
					item.h		    		item.cpp \
					items.h		    		items.cpp \
					font.h		    		font.cpp \
					game.h		    		game.cpp \
					geometry.h		    	geometry.cpp \
					plane.h		    		plane.cpp \
					PrefsManager.h		    PrefsManager.cpp \
					Singleton.h				mainOS.cpp \
					surfaceDB.h		    	surfaceDB.cpp \
					SDLMain.h		    	SDLMain.m \
					video.h		    		video.cpp

HF_CXXFLAGS=-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -I/Library/Frameworks/SDL.framework/Headers -I/Library/Frameworks/SDL_image.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers
HF_OBJCFLAGS=-I/Library/Frameworks/SDL.framework/Headers -I/Library/Frameworks/SDL_image.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers
HF_LDADD=
HF_LDFLAGS= -framework SDL -framework SDL_image -framework Cocoa -framework OpenGL
