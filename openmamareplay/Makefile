SRCDIR=..
MAMA=1
ZLIB=1
DEBUG=1

MAMA_HOME=/opt/libraries/mama/
WOMBAT_PATH=/home/frank/OpenMAMA/16-Feb-2012/config

# Target specific variables

test: REPLAY_TEST_ARGS=-S TEST -tport publish -m wmw -f ../playback.play.gz -DF ../openmamacapture/data.dict -rewind -1 -rate 500

include $(SRCDIR)/makeinc/Makefile.common

ALL_OBJS = openmamareplay$(O) \
	$(SRCDIR)/framework/GzFileHandler$(O) \
	$(SRCDIR)/framework/MamaRecordedMessage$(O) \
	$(SRCDIR)/framework/MamaRecordedMessageCodec$(O) \
	$(SRCDIR)/framework/PlaybackDQPublisherCallback$(O) \
	$(SRCDIR)/framework/GenericMamaCache$(O) \
	$(SRCDIR)/framework/GenericMamaUtilities$(O) 


openmamareplay: $(ALL_OBJS)
	$(LINK.C) -o $@ $^ $(LIBS)

test:
	WOMBAT_PATH=$(WOMBAT_PATH) ./openmamareplay $(REPLAY_TEST_ARGS)

clean: 
	$(RM) $(ALL_OBJS) openmamareplay

