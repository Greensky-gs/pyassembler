pkgname=pyassembler
_name=pyassembler
pkgver=1.0.0
pkgrel=1
pkgdesc="A simple script that takes all python files in a given folder to assemble them in a single python file"
arch=('x86_64' 'aarch64')
url="https://git.greensky.tf/Greensky/pyassembler"
license=("unknown")
depends=()
makedepends=('git' 'gcc' 'make')
optdepends=()
source=("git+${url}.git#commit=ce225bd8bbd5725a9eb796fbdba8a069ae1d3fe0")
options=('!debug')
sha256sums=("SKIP")

build() {
	cd "$srcdir/$_name"

	make cleanbuild
}

package() {
	cd "$srcdir/$_name"

	install -Dm755 bin/main.uwu "$pkgdir/usr/bin/pyassembler"

	install -Dm644 docs/pyassembler.1 "$pkgdir/usr/share/man/man1/pyassembler.1"
}
