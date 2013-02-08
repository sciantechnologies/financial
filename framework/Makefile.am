lib_LTLIBRARIES	=	libscianfinancial.la

LIBS		+=	-lwombatcommon -lmama -lmamacpp -lz

AM_LDFLAGS	=	-no-undefined

libscianfinancial_la_SOURCES	=	\
		GenericMamaCache.cpp \
		GenericMamaCache.h \
		GenericMamaManager.cpp \
		GenericMamaManager.h \
		GenericMamaUtilities.cpp \
		GenericMamaUtilities.h \
		GenericUtilities.cpp \
		GenericUtilities.h \
		GzFileHandler.cpp \
		GzFileHandler.h \
		Logger.cpp \
		Logger.h \
		Mama2RssEngine.cpp \
		Mama2RssEngine.h \
		MamaCachingCallback.cpp \
		MamaCachingCallback.h \
		MamaRecordedMessage.cpp \
		MamaRecordedMessage.h \
		MamaRecordedMessageCodec.cpp \
		MamaRecordedMessageCodec.h \
		PlaybackDQPublisherCallback.cpp \
		PlaybackDQPublisherCallback.h \
		RecordingMamaSubscriptionCallback.cpp \
		RecordingMamaSubscriptionCallback.h 

clean:
	rm -rf .libs _libs
	rm -f *.o
	rm -f *.lo

