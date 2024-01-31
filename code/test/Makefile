CC=gcc
CFLAGS=`pkg-config --cflags glib-2.0 sqlite3 gmp`
LIBS=`pkg-config --libs glib-2.0 sqlite3 gmp` 
BUILDDIR=../build
PROJECTLIBDIR=-L../build/lib
PROJECTLIB=-lvoting


$(shell mkdir -p $(BUILDDIR))

all: $(BUILDDIR)/test_electeur $(BUILDDIR)/test_election $(BUILDDIR)/main $(BUILDDIR)/test_crypto2 $(BUILDDIR)/test_election_vote $(BUILDDIR)/test_date


$(BUILDDIR)/test_electeur: test_electeur.c
	$(CC) test_electeur.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_electeur

$(BUILDDIR)/test_election: test_election.c
	$(CC) test_election.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_election

$(BUILDDIR)/main: main.c
	$(CC) main.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/main

$(BUILDDIR)/test_future: test_future.c
	$(CC) test_future.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_future

$(BUILDDIR)/test_crypto2: test_crypto2.c
	$(CC) test_crypto2.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_crypto2

$(BUILDDIR)/test_election_vote: test_election_vote.c
	$(CC) test_election_vote.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_election_vote

$(BUILDDIR)/test_date: test_date.c
	$(CC) test_date.c $(CFLAGS) $(PROJECTLIBDIR) $(PROJECTLIB) $(LIBS) -o $(BUILDDIR)/test_date

clean:
	rm $(BUILDDIR)/test_election; rm $(BUILDDIR)/test_electeur 
