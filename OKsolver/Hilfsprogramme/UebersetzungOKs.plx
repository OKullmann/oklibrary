#!/usr/bin/perl

# Oliver Kullmann, 8.1.2002 (Swansea)

# Aufruf:
#
# UebersetzungOKs.plx [-D=0/1] [-I=0/1] [-Z=0/1] [-S=0/1] [-F=*.*] [-B=*]
# (in beliebiger Reihenfolge, und auch mit Wiederholungen)
#
# Siehe die "Laufende Dokumentation zu OKsolver".

use strict;
use strict 'refs';

use lib glob("/h/21/GemeinsameBasis/ZentralesArchiv/Hilfsprogramme/PerlBibliothek");

use Dateibearbeitung;
use DateiTests;
use Listen;
use Zeichenketten;


use constant HauptVersionsnummer => 1;
use constant UnterVersionsnummer => 2;
use constant AnzahlSprachen => 2;
use constant Namenskuerzel => "OKs";
use constant Uebersetzer => "g++";
use constant FesteUebersetzungsOpt => "-Wall -pedantic -ansi -Winline -lm";

my $MaxIndexSprachen = AnzahlSprachen - 1;
my $Selbst = "UebersetzungOKs.plx";

# Bestimmung der Argumente

my $Basisordner = glob("/h/21/GemeinsameBasis/SAT-Algorithmen/OKsolver");

my ($Dynamisch, $Inline, $Systemzeit, $Sprache, $Faktor) = (0, 1, 0, 0, "1.0");

for (my $i = 0; $i < Listen::Laenge(@ARGV); $i++) {
  if ($ARGV[$i] =~ /-.*/) {
    if ($ARGV[$i] =~ /-D=([01])/) {
      $Dynamisch = $1;
    }
    elsif ($ARGV[$i] =~ /-I=([01])/) {
      $Inline = $1;
    }
    elsif ($ARGV[$i] =~ /-Z=([01])/) {
      $Systemzeit = $1;
    }
    elsif ($ARGV[$i] =~ /-S=([0-$MaxIndexSprachen])/) {
      $Sprache = $1;
    }
    elsif ($ARGV[$i] =~ /-F=(\d+\.\d+)/) {
      $Faktor = "$1";
    }
    elsif ($ARGV[$i] =~ /-B=(\S+)/) {
      $Basisordner = "$1";
    }
    else {
      die "FEHLER[$Selbst]: Das Argument \"$ARGV[$i]\" ist keine gueltige Option!\n";
    }
  }
  else {
    $Basisordner = $ARGV[$i];
  }
}

# Initialisierung der Resourcen

$Basisordner = glob($Basisordner);
my $UebersetzungsdatenOrdner = "$Basisordner/Uebersetzungsdaten";

my %RsNa;
my %RsSt;
my %RsDo;

$RsNa{"Basis"} = "$Basisordner";
$RsSt{"Basis"} = "dxw";
$RsDo{"Basis"} = "Basisordner fuer OKsolver";

$RsNa{"OKsOrdner"} = "$Basisordner/Quellen";
$RsSt{"OKsOrdner"} = "dx";
$RsDo{"OKsOrdner"} = "Ordner, der die Quelltext-Dateien enthaelt";

$RsNa{"LaufNr"} = "$UebersetzungsdatenOrdner/LaufendeNummer";
$RsSt{"LaufNr"} = "frw";
$RsDo{"LaufNr"} = "Laufende Nummer der Uebersetzung";

$RsNa{"DateiListe"} = "$UebersetzungsdatenOrdner/Programmdateien";
$RsSt{"DateiListe"} = "fr";
$RsDo{"DateiListe"} = "Liste der Namen der Quelltextdateien";

$RsNa{"Optionen"} = "$UebersetzungsdatenOrdner/UebersetzerOptionen";
$RsSt{"Optionen"} = "fr";
$RsDo{"Optionen"} = "Datei mit den Uebersetzer-Optionen in der ersten Zeile";

$RsNa{"Abstand"} = "$UebersetzungsdatenOrdner/Abstandsfunktion";
$RsSt{"Abstand"} = "fr";
$RsDo{"Abstand"} = "Datei mit dem Namen der Abstandsfunktion in der ersten Zeile";

$RsNa{"Projektion"} = "$UebersetzungsdatenOrdner/Projektionsfunktion";
$RsSt{"Projektion"} = "fr";
$RsDo{"Projektion"} = "Datei mit dem Namen der Projektionsfunktion in der ersten Zeile";

$RsNa{"SAs"} = glob("$Basisordner/../SAs/OKsolver");
$RsSt{"SAs"} = "dwx";
$RsDo{"SAs"} = "Verzeichnis, wo hinein der Ordner mit den uebersetzten Programmen gesetzt wird";

$RsNa{"Protokoll"} = "$Basisordner/UebersetzungsProtokoll";
$RsSt{"Protokoll"} = "fa";
$RsDo{"Protokoll"} = "Datei, in die das Uebersetzungsprotokoll geschrieben wird (an's Ende)";

$RsNa{"Meldungen"} =  "$Basisordner/UebersetzungsMeldungen";
$RsSt{"Meldungen"} =  "w";
$RsDo{"Meldungen"} = "Datei, in die Meldungen zur Uebersetzung geschrieben werden";

if ($Inline) {
  $RsNa{"GesamtOKs"} = "$Basisordner/GesamtOKs.cpp";
  $RsSt{"GesamtOKs"} =  "w";
  $RsDo{"GesamtOKs"} = "Datei mit allen Uebersetzungs-Einheiten zusammengestellt";
}


my %RsIn := DateiTests::Initialisierung(\%RsNa, \%RsSt, \%RsDo);


# -----------------------------

# Bestimmen der laufenden Nummer

my $LaufendeNr;
{
  my $Name = $RsNa{"LaufNr"};
  if (! (${$RsIn{"LaufNr"}}[0] =~ /^\s*(\d+)+\s*$/)) {
    die "FEHLER[$Selbst]: Die erste Zeile der Datei $Name muss aus einer natuerlichen Zahl bestehen!";
  }
  $LaufendeNr = $1;
  $LaufendeNr++;
  open INHALT, "> $Name" or die "FEHLER[$Selbst]: Es muss die Datei $Name schreibbar sein!\n";
  print INHALT $LaufendeNr;
  close INHALT;
}


# Bestimmen und Testen der Programmdateien

my @Programmdateien = @{$RsIn{"DateiListe"}};

for (my $i = 0; $i < Listen::Laenge(@Programmdateien); $i++) {
  if ($Programmdateien[$i] eq "Abstand") {
    $Programmdateien[$i] = ${$RsIn{"Abstand"}}[0];
  }
  elsif ($Programmdateien[$i] eq "Projektion") {
    $Programmdateien[$i] = ${$RsIn{"Projektion"}}[0];
  }
  $Programmdateien[$i] = $RsNa{"OKsOrdner"} . "/$Programmdateien[$i]";
}

DateiTests::SetzenStatus("fr");
{
  my @Liste = &DateiTests::Test(@Programmdateien);
  if (! Listen::Leer(@Liste)) {
    print "FEHLER[$Selbst]: Alle Dateien aufgefuehrt in " . $RsNa{"DateiListe"} . " muessen existieren und lesbar sein, nicht so jedoch die Dateien:\n";
    print "@Liste" . "\n";
    die;
  }
}


my $AllgOptionen = ${$RsIn{"Optionen"}}[0];

# Die speziellen OKsolver-Optionen

my @OptKomb;
my @OptBez;

$OptKomb[0] = "";
$OptBez[0] = "T";

$OptKomb[1] = "-DLITTAB";
$OptBez[1] = "TT";

$OptKomb[2] = "-DNBAUMRES -DLOKALLERNEN";
$OptBez[2] = "L2C";

$OptKomb[3] = "-DNBAUMRES -DLOKALLERNEN -DFASTAUTARKIE";
$OptBez[3] = "L2C1A";

$OptKomb[4] = "-DNBAUMRES -DLOKALLERNEN -DFASTAUTARKIE -DNL2RED";
$OptBez[4] = "L1A";

$OptKomb[5] = "-DNBAUMRES -DLOKALLERNEN -DNL2RED";
$OptBez[5] = "L";

$OptKomb[6] = "-DNBAUMRES";
$OptBez[6] = "N";

$OptKomb[7] = "-DOUTPUTTREEDATAXML";
$OptBez[7] = "XT";

my $AnzVers = Listen::Laenge(@OptKomb);


# Die allgemeinen OKsolver-Optionen

my $AllgOKsOptionen;
my $AllgOKsOptBez = "";
{
  my @ListeOptionen;
  if ($Dynamisch) {
    push(@ListeOptionen, "-DDYNAMISCH");
    push(@ListeOptionen, "-DFAKTOR=$Faktor");
    $AllgOKsOptBez = $AllgOKsOptBez . "D";
  }
  if (! $Inline) {
    push(@ListeOptionen, "-DKEININLINE");
  }
  else {
    $AllgOKsOptBez = $AllgOKsOptBez . "I";
  }
  if ($Systemzeit) {
    push(@ListeOptionen, "-DSYSTIME");
    $AllgOKsOptBez = $AllgOKsOptBez . "S";
  }
  if ($Sprache != 0) {
    push(@ListeOptionen, "-DSPRACHE=$Sprache");
  }
  $AllgOKsOptionen =  Zeichenketten::Konkat(@ListeOptionen);
}


# Erzeugen der Liste der Quell-Dateien fuer den Uebersetzer
# (falls Inline gesetzt, so wird eine Gesamt-Datei erzeugt)

my $QuellListe;

if ($Inline) {
  Dateibearbeitung::Ausgiessen(@Programmdateien, $RsNa{"GesamtOKs"});
  $QuellListe = $RsNa{"GesamtOKs"};
}
else {
  $QuellListe =  Zeichenketten::Konkat(@Programmdateien);
}


# Erzeugen der Versions-Namen

my $Basisname = Namenskuerzel . HauptVersionsnummer . "_" . UnterVersionsnummer . "_" .  sprintf("%05d", $LaufendeNr);
if ($AllgOKsOptBez ne "") {
  $Basisname = $Basisname . "_" . $AllgOKsOptBez;
}

my @Versionsnamen;
for (my $i = 0; $i < $AnzVers; $i++) {
  $Versionsnamen[$i] = $Basisname . "_" . $OptBez[$i] . ".sa";
}


# Praeparation des Ordners fuer die uebersetzten Programme

my $Rechner = $ENV{"HOST"};
my $OKsProgOrdner = $RsNa{"SAs"} . "/$Basisname" . "_$Rechner";
if (-e $OKsProgOrdner) {
  die "FEHLER[$Selbst]: Der Ordner $OKsProgOrdner zur Aufnahme der uebersetzten OKsolver-Versionen existiert schon!\n";
}
mkdir $OKsProgOrdner or die "FEHLER[$Selbst]: Der Ordner $OKsProgOrdner zur Aufnahme der uebersetzten OKsolver-Versionen konnte nicht angelegt werden!\n";


# Erzeugen der Uebersetzungs-Kommandos

my @Uebersetzerzeilen;
for (my $i = 0; $i < $AnzVers; $i++) {
  $Uebersetzerzeilen[$i] = Zeichenketten::Konkat(Uebersetzer, FesteUebersetzungsOpt, "-o $OKsProgOrdner/$Versionsnamen[$i]", "-I" . $RsNa{"OKsOrdner"}, $AllgOKsOptionen, $OptKomb[$i], $AllgOptionen, $QuellListe);
}



# Uebersetzen und protokollieren

{
  my $Datum = localtime;
  my $MDatei = $RsNa{"Meldungen"};
  my $PDatei = $RsNa{"Protokoll"};
  open INHALT, "> $MDatei" or die "FEHLER[$Selbst]: Die Datei $MDatei konnte nicht initialisiert werden!\n";
  close INHALT;
  Dateibearbeitung::Anhaengen("$Rechner: $Selbst $Datum\n", $MDatei);
  Dateibearbeitung::Anhaengen("\n--------\n\n$Rechner: $Selbst $Datum\n", $PDatei);
  if ($Inline) {
    Dateibearbeitung::Anhaengen("\nQuelldateien:\n" . Zeichenketten::Konkat(@Programmdateien) . "\n", $PDatei);
  }
  for (my $i = 0; $i < $AnzVers; $i++) {
    Dateibearbeitung::Anhaengen("\n" . $Uebersetzerzeilen[$i] . "\n", $MDatei);
    Dateibearbeitung::Anhaengen("\n" . $Uebersetzerzeilen[$i] . "\n", $PDatei);
    ! system($Uebersetzerzeilen[$i] . " 2>> $MDatei") or die "FEHLER[$Selbst]: Uebersetzungsfehler mit \n$Uebersetzerzeilen[$i]\n";
  }
}

