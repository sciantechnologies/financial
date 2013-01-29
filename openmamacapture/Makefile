SRCDIR=..
MAMA=1
ZLIB=1

MAMA_HOME=/opt/libraries/mama/

include $(SRCDIR)/makeinc/Makefile.common

ALL_OBJS = openmamacapture$(O) \
	$(SRCDIR)/framework/RecordingMamaSubscriptionCallback$(O) \
	$(SRCDIR)/framework/GzFileHandler$(O) \
	$(SRCDIR)/framework/MamaRecordedMessageCodec$(O)

openmamacapture: $(ALL_OBJS)
	$(LINK.C) -o $@ $^ $(LIBS)

clean: 
	$(RM) $(ALL_OBJS) openmamacapture

