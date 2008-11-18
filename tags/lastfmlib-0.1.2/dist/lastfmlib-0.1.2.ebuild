# Copyright 1999-2008 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit autotools eutils

DESCRIPTION="C++ library to scrobble tracks on Last.fm"
HOMEPAGE="http://code.google.com/p/lastfmlib"
SRC_URI="http://${PN}.googlecode.com/files/${P}.tar.gz"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="alpha amd64 arm hppa ia64 ppc ppc64 sparc x86"
IUSE="logging test"

RDEPEND="net-misc/curl
         logging? ( dev-libs/log4cpp )
         unittests? ( dev-cpp/unittest++ )"
DEPEND="${RDEPEND}"

src_compile() {
    econf \
        $(use_enable logging) \
        $(use_enable unittests) || die

    emake || die "emake failed."
}

src_install() {
    emake DESTDIR="${D}" install || die "emake install failed."
    dodoc AUTHORS ChangeLog README TODO
}
