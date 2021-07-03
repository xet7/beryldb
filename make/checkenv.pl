#!/usr/bin/env perl
#
# BerylDB - A modular database.
# http://www.beryldb.com
#
# Copyright (C) 2021 Carlos F. Ferry <cferry@beryldb.com>
# 
# This file is part of BerylDB. BerylDB is free software: you can
# redistribute it and/or modify it under the terms of the BSD License
# version 3.
#
# More information about our licensing can be found at https://docs.beryl.dev

use strict;
use warnings FATAL => qw(all);

use File::Basename qw(basename dirname);
use FindBin        qw($RealDir);

use lib dirname $RealDir;
use make::utils;

use constant 
{
	BUILDPATH  => $ENV{BUILDPATH},
	SOURCEPATH => $ENV{SOURCEPATH}
};

sub locate_outputs;
sub gendep($);
sub dep_cpp($$$);
sub dep_so($);
sub dep_dir($$);
sub run();

my %f2dep;

run;
exit 0;

sub run() 
{
	create_directory(BUILDPATH, 0770) or die "Unable to create directory: $!";
	chdir BUILDPATH or die "Unable to open directory: $!";
	unlink 'include';
	symlink "${\SOURCEPATH}/include", 'include';
	mkdir $_ for qw/bin modules brldb managers obj/;

	open MAKE, '>real.mk' or die "Unable to write pre parser: $!";
	chdir "${\SOURCEPATH}/src";

	run_dynamic();
	close MAKE;
}

sub run_dynamic() 
{
	print MAKE <<END;
VPATH = \$(SOURCEPATH)/src

bad-target:
	\@echo "This Makefile must be run by a sub-make from the source"
	\@echo "in order to set the correct environment variables"
	\@exit 1

all: beryldb modules

END
	my(@core_deps, @modlist);
	
	for my $file (<brldb/*.cpp>, <managers/*.cpp>, <*.cpp>, <engines/*.cpp>) 
	{
		my $out = locate_outputs $file;
		dep_cpp $file, $out, 'gen-o';
                next if $file =~ m#^engines/# && $file ne "engines/engine_$ENV{ENGINE}.cpp";

		if ($file =~ /^(m|core)_.*\.cpp/) 
		{
			my $correctsubdir = ($file =~ /^m_/ ? "modules" : "coremods");
			print "Error: module $file is in the src directory, put it in src/$correctsubdir instead!\n";
			exit 1;
		}
		
		push @core_deps, $out;
	}

	for my $directory (qw(coremods modules)) 
	{
		opendir(my $moddir, $directory);
		
		for my $file (sort readdir $moddir) 
		{
			next if $file =~ /^\./;

			if ($directory eq 'modules' && -e "modules/extra/$file" && !-l "modules/$file") 
			{
				print "Replacing symlink for $file found in modules/extra\n";
				rename "modules/$file", "modules/$file~";
				symlink "extra/$file", "modules/$file";
			}
			
			if ($file =~ /^(?:core|m)_/ && -d "$directory/$file" && dep_dir "$directory/$file", "modules/$file") 
			{
				mkdir "${\BUILDPATH}/obj/$file";
				push @modlist, "modules/$file.so";
			}
			
			if ($file =~ /^.*\.cpp$/) 
			{
				my $out = dep_so "$directory/$file";
				push @modlist, $out;
			}
		}
	}

	my $core_mk = join ' ', @core_deps;
	my $mods = join ' ', @modlist;
	print MAKE <<END;

bin/beryldb: $core_mk
	@\$(SOURCEPATH)/make/links.pl core-ld \$\@ \$^ \$>

beryldb: bin/beryldb

modules: $mods

.PHONY: all bad-target beryldb modules

END
}

sub locate_outputs 
{
	my $file = shift;
	my($path,$base) = $file =~ m#^((?:.*/)?)([^/]+)\.cpp# or die "Bad file $file";
	
	if ($path eq 'modules/' || $path eq 'coremods/') 
	{
		return "modules/$base.so";
	} 
	elsif ($path eq '' || $path =~ /^engines\/$/) 
	{
		return "obj/$base.o";
	} 
	elsif ($path =~ m#modules/(m_.*)/# || $path =~ m#coremods/(core_.*)/#)
	{
		return "obj/$1/$base.o";
	} 
	elsif ($path =~ m#brldb/(.*)#)
	{
                return "brldb/$1/$base.o";
	}
	elsif ($path =~ m#managers/(.*)#)
	{
		return "managers/$1/$base.o";
	}
	else 
	{
		die "Can't determine output for $file";
	}
}

sub gendep($) 
{
	my $f = shift;
	my $basedir = $f =~ m#(.*)/# ? $1 : '.';
	return $f2dep{$f} if exists $f2dep{$f};
	$f2dep{$f} = '';
	my %dep;
	my $link = readlink $f;
	
	if (defined $link) 
	{
		$link = "$basedir/$link" unless $link =~ m#^/#;
		$dep{$link}++;
	}
	
	open my $in, '<', $f or die "Could not read $f";

	while (<$in>) 
	{
		if (/^\s*#\s*include\s*"([^"]+)"/) 
		{
			my $inc = $1;
			next if $inc eq 'constants.h' && $inc eq 'config.h' && $f eq '../include/beryl.h';
			my $found = 0;

			for my $loc ("$basedir/$inc", "../include/$inc") 
			{
				next unless -e $loc;
				$found++;
				$dep{$_}++ for split / /, gendep $loc;
				$loc =~ s#^\.\./##;
				$dep{$loc}++;

			}
			if ($found == 0) 
			{
				print STDERR "Unable to find header: $inc for $f\n";
			} 
			elsif ($found > 1 && $basedir ne '../include') 
			{
				print STDERR "Undefined include $inc in $f\n";
			}
		}
	}

	close $in;
	$f2dep{$f} = join ' ', sort keys %dep;
	$f2dep{$f};
}

sub dep_cpp($$$) 
{
	my($file, $out, $type) = @_;
	gendep $file;

	print MAKE "$out: $file $f2dep{$file}\n";
	print MAKE "\t@\$(SOURCEPATH)/make/links.pl $type \$\@ \$(SOURCEPATH)/src/$file \$>\n";
}

sub dep_so($) 
{
	my($file) = @_;
	my $out = locate_outputs $file;

	my $name = basename $out, '.so';
	print MAKE ".PHONY: $name\n";
	print MAKE "$name: $out\n";

	dep_cpp $file, $out, 'gen-so';
	return $out;
}

sub dep_dir($$) 
{
	my($dir, $outdir) = @_;
	my @ofiles;
	opendir DIR, $dir;
	
	for my $file (sort readdir DIR) 
	{
		next unless $file =~ /(.*)\.cpp$/;
		my $ofile = locate_outputs "$dir/$file";
		dep_cpp "$dir/$file", $ofile, 'gen-o';
		push @ofiles, $ofile;
	}
	
	closedir DIR;
	
	if (@ofiles) 
	{
		my $ofiles = join ' ', @ofiles;
		my $name = basename $outdir;
		print MAKE ".PHONY: $name\n";
		print MAKE "$name: $outdir.so\n";
		print MAKE "$outdir.so: $ofiles\n";
		print MAKE "\t@\$(SOURCEPATH)/make/links.pl link-dir \$\@ ${\SOURCEPATH}/src/$dir \$^ \$>\n";
		return 1;
	} 
	else 
	{
		return 0;
	}
}

