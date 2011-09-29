AM_CXXFLAGS=-g

bin_PROGRAMS = HF

HF_SOURCES =    background.h		    background.cpp \
					item.h		    		item.cpp \
					items.h		    		items.cpp \
					font.h		    		font.cpp \
					Score.h		    		Score.cpp \
					game.h		    		game.cpp \
					geometry.h		    	geometry.cpp \
					plane.h		    		plane.cpp \
					PrefsManager.h		    PrefsManager.cpp \
					Singleton.h				mainOS.cpp \
					TextureManager.h		TextureManager.cpp \
					SoundManager.h			SoundManager.cpp \
					SDLMain.h		    	SDLMain.m \
					video.h		    		video.cpp \
					ofxOsc/src/ofxOsc.h		ofxOsc/src/ofxOscArg.h \
					ofxOsc/src/ofxOscBundle.h		ofxOsc/src/ofxOscBundle.cpp \
					ofxOsc/src/ofxOscMessage.h		ofxOsc/src/ofxOscMessage.cpp \
					ofxOsc/src/ofxOscReceiver.h		ofxOsc/src/ofxOscReceiver.cpp \
					ofxOsc/src/ofxOscSender.h		ofxOsc/src/ofxOscSender.cpp \
					ofxOsc/libs/oscpack/include/ip/IpEndpointName.h \
					ofxOsc/libs/oscpack/include/ip/IpEndpointName.cpp \
					ofxOsc/libs/oscpack/include/ip/posix/NetworkingUtils.cpp \
					ofxOsc/libs/oscpack/include/ip/posix/UdpSocket.cpp \
					ofxOsc/libs/oscpack/include/ip/NetworkingUtils.h \
					ofxOsc/libs/oscpack/include/ip/PacketListener.h \
					ofxOsc/libs/oscpack/include/ip/TimerListener.h \
					ofxOsc/libs/oscpack/include/ip/UdpSocket.h \
					ofxOsc/libs/oscpack/include/osc/MessageMappingOscPacketListener.h \
					ofxOsc/libs/oscpack/include/osc/OscException.h \
					ofxOsc/libs/oscpack/include/osc/OscHostEndianness.h \
					ofxOsc/libs/oscpack/include/osc/OscOutboundPacketStream.h \
					ofxOsc/libs/oscpack/include/osc/OscOutboundPacketStream.cpp \
					ofxOsc/libs/oscpack/include/osc/OscPacketListener.h \
					ofxOsc/libs/oscpack/include/osc/OscPrintReceivedElements.h \
					ofxOsc/libs/oscpack/include/osc/OscPrintReceivedElements.cpp \
					ofxOsc/libs/oscpack/include/osc/OscReceivedElements.h \
					ofxOsc/libs/oscpack/include/osc/OscReceivedElements.cpp \
					ofxOsc/libs/oscpack/include/osc/OscTypes.h \
					ofxOsc/libs/oscpack/include/osc/OscTypes.cpp
					


HF_CXXFLAGS= -DOPENGL -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS -I/Library/Frameworks/SDL.framework/Headers -I/Library/Frameworks/SDL_image.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers -IofxOsc/src -IofxOsc/libs/oscpack/include -IofxOsc/libs/oscpack/include/ip -IofxOsc/libs/oscpack/include/osc
HF_OBJCFLAGS=-I/Library/Frameworks/SDL.framework/Headers -I/Library/Frameworks/SDL_image.framework/Headers -I/System/Library/Frameworks/OpenGL.framework/Headers
HF_LDADD=
HF_LDFLAGS= -framework SDL -framework SDL_image -framework Cocoa -framework OpenGL 
