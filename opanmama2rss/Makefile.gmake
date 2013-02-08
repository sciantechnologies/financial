SRCDIR=..
MAMA=1
ZLIB=1
DEBUG=1

WOMBAT_PATH=/home/frank/OpenMAMA/16-Feb-2012/config

# Add mongoose includes
INCLUDES += -I$(SRCDIR)/thirdparty/
#MAMA_HOME=/opt/libraries/mama/
MAMA_HOME="/cygdrive/c/Users/fquinn/My Documents/Scian/openmama-install/"

include $(SRCDIR)/makeinc/Makefile.common


ALL_OBJS = mama2rss$(O) \
	$(SRCDIR)/thirdparty/mongoose/mongoose$(O) \
	$(SRCDIR)/framework/GenericMamaManager$(O) \
	$(SRCDIR)/framework/MamaCachingCallback$(O) \
	$(SRCDIR)/framework/Mama2RssEngine$(O) \
	$(SRCDIR)/framework/GenericUtilities$(O)

all: mama2rss


mama2rss: $(ALL_OBJS)
	$(LINK.C) -o $@ $^ $(LIBS)

clean: 
	$(RM) $(ALL_OBJS) mama2rss

test: mama2rss
	WOMBAT_PATH=$(WOMBAT_PATH) ./mama2rss -m wmw -p 8080 -t ./rss_template.xml -DF ../openmamacapture/data.dict

