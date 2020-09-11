# Errata

The libESMTP git repository has been rebuilt by hand from the collection of
released tarballs. A few errors or inconsistencies occurred during this
process meaning that a tarball built from the repository will differ from the
original.

The intention was not to provide byte-identical record of released tarballs,
rather, it was to be able to review how source code changed should the need
arise. For example, there was a regression in X.509 certificate validation at
one point.  Consequently only the latest release, v1.0.6, was properly
validated as buildable.

The majority of differences are minor, most are due to the decision to exclude
generated files from the repository (autotools pollutes the source with a large
number of files).

## Minor Differences

Files generated by GNU autotools and associated tools such as `libtoolize`
were removed, with the following caveats:

* `acinclude.m4` should have been removed but was not.
* `stamp-h.in` should have been removed but was not.
* `libesmtp.spec` was removed but accidentally added back in v0.8.1. Removed
  again in v0.8.7.
* `acconfig.h` is generated from v0.8.2 but was not removed from the repository.

v0.8.5 renamed `rfc822date.[ch]` to `rfc2822date.[ch]`.  The latter were added
as new files to the repository rather than using `git mv`. The old files were
not removed until revision `bdbc6c2c15e7`.

The files COPYING and COPYING.GPL are incorrect wrt the tarballs from v1.0.4.

## configure.in

A more serious error occurred when the v0.8.5 tarball reverted `configure.ac`
to `configure.in` due to autoconf version problems (who knew?).
Here is the comment from `Changelog`:

>    configure.in acinclude.m4 acconfig.h
>
>    Reverted to configure.in, reinstated acconfig.h and added
>    some compatibility stuff to acinclude.m4.  All this to try
>    and be compatible with autoconf 2.5 *and* 2.13.  I really
>    hate autoconf.

This step was unfortunately missed when adding v0.8.5 to the repository.
If you need to build previous releases for any reason this is a problem
as the correct revision of `configure.in` is missing from the repository.

`configure.ac` was reinstated in v1.0.6. Unfortunately this was not noticed as
only v1.0.6 was validated to build as noted above. My personal view is that
if older versions of libESMTP must be built then it would be significantly
easier to back-port the Meson build files.

## Remedial Action

Since it is not recommended to use versions of libESMTP prior to 1.0.6 no
action is taken to correct these.  The notes above should be sufficient to
correctly build earlier versions if absolutely required.

A branch for v1.0.6 is created and patched to ensure consistency with the
widely distributed tarball, however README is updated to the current
repository.

Since autotools is completely removed and libESMTP migrated to Meson at
revision `5ea5d5ec7c7f` no further action is taken.