#  $sysrecipes: Makefile,v 1.0 2026/11/03 14:34:10 littlefly365 Exp $

PREFIX ?= /usr
BINDIR ?= $(PREFIX)/bin
RECIPEDIR ?= $(PREFIX)/sysrecipes
SYSCONFDIR ?= /etc

all:

install: install-exec install-recipes

install-exec: install-files
	install -Dm 755 mkpkg $(DESTDIR)$(BINDIR)/mkpkg
install-files:
	install -Dm 644 mkpkg.conf $(DESTDIR)$(SYSCONFDIR)/mkpkg.conf
install-recipes:
	install -Dm 644 LFS $(DESTDIR)$(RECIPEDIR)/LFS
.PHONY: all
