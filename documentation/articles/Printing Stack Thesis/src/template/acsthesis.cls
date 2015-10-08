\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{acsthesis}[2015/05/21 Template for Student Theses at ACS]

\LoadClass[12pt, 				% Fontsize
		   a4paper,				% Paper format
		   listof=totoc,		% 'List of Tables' and 'List of Figures' in table of contents (toc)
		   numbers=noenddot, 	% Deactivate dot behind last number in toc
		   index=totoc,			% Add index to toc
		   BCOR1cm,				% additional margin for binding
		   ]{scrbook}

%------------------------------------------------------------------------------%
% Include packages
%------------------------------------------------------------------------------%
\RequirePackage[utf8]{luainputenc} 		% Enconding of the tex file
\RequirePackage{cmap} 						% Nice ligatures
\RequirePackage[T1]{fontenc} 				% Encoding of the output PDF
\RequirePackage{textcomp} 					% ????
\RequirePackage{lmodern} 					% Use modern Linux fonts
\RequirePackage{graphicx} 					% Include graphics via 'includegraphic'
\RequirePackage{makeidx} 					% Create an index if desired
\RequirePackage[binary-units]{siunitx} 		% Nice numbers with units
\RequirePackage{hyperref} 					% Links within document
\RequirePackage{xparse} 					% NewDocumentCommand etc.
\RequirePackage{etoolbox} 					% If-then-else
\RequirePackage{ifthen} 					% If-then-else
\RequirePackage{babel} 						%
\RequirePackage{booktabs} 					% nicer tables
\RequirePackage{tikz} 						% draw pictures
\RequirePackage{pgfplots} 					% use for printing diagrams 
\RequirePackage{listings}

%------------------------------------------------------------------------------%
% Command and environment definitions
%------------------------------------------------------------------------------%
\newcommand{\addinstitute}[3]{}                 % Institute hinzufuegen
                                                %  #1 Kuerzel fuer Referenz, z.B. lfbs
                                                %  #2 Titel des Instituts, z.B. Chair for OS, RWTH Aachen University
                                                %  #3 Referenzzeichen, z.B. (*) oder irgendein Symbol
%\newcommand*{\instituteref}[1]{}                % Gibt das oben definierte Symbol zum Kuerzel aus
%\newcommand{\addsupervisor}[1]{}                % Betreuer hinzufuegen (Optional Institutskuerzel)
%\newcommand{\addsecondreferee}[1]{}             % Zweitgutachter hinzufuegen (Optional Institutskuerzel)


\newcommand{\printinstitutes}{}
\newcommand{\printsupervisors}{}
\newcommand{\printsecondreferees}{}

% ----- Abstract (german and english)
\addto\captionsgerman{%
	\RenewDocumentCommand \abstractname {} {Kurzfassung}
	\def\@keywordName{Stichw\"orter}
	\def\@keywords{\@keywordsDE}
}
\addto\captionsngerman{%
	\RenewDocumentCommand \abstractname {} {Kurzfassung}
	\def\@keywordName{Stichw\"orter}
	\def\@keywords{\@keywordsDE}
}
\addto\captionsenglish{%
	\def\@keywordName{Keywords}
	\def\@keywords{\@keywordsEN}
}

\NewDocumentEnvironment{abstract}{}%
{
	\addchap*{\abstractname}
}%
{%end-defs
	\bigskip\par
	\textbf{\@keywordName:} \@keywords
}

% ----- User commands
\def\@titleEN{\@title}
\def\@thesisType{Unknown Thesis Type}
\NewDocumentCommand \titleDE         { m } {\def\@titleDE{#1}}
\NewDocumentCommand \titleEN         { m } {\def\@titleEN{#1}}
\NewDocumentCommand \matrNr          { m } {\def\@matrNr{#1}}
\NewDocumentCommand \submissionDate  { m } {\def\@submissionDate{#1}}
\NewDocumentCommand \addSupervisor   { m } {\edef\@supervisors{#1}}
\NewDocumentCommand \keywordsDE      { m } {\def\@keywordsDE{#1}}
\NewDocumentCommand \keywordsEN      { m } {\def\@keywordsEN{#1}}
\NewDocumentCommand \thesisType      { m } {%
	\ifstrequal{#1}{master}{%
		\iflanguage{ngerman}{%
			\def\@thesisType{Masterarbeit}
		}{%
			\def\@thesisType{Master Thesis}
		}
	}{%
	}
	\ifstrequal{#1}{bachelor}{%
		\iflanguage{ngerman}{%
			\def\@thesisType{Bachelorarbeit}
		}{%
			\def\@thesisType{Bachelor Thesis}
		}
	}{%
	}
}


%
% Institut hinzufuegen (falls mehrere Institute/Firmen involviert)
%  #1 Kuerzel (zur Verwendung mit \instituteref)
%  #2 Kompletter Name des Instituts
%  #3 Symbol zur Kennzeichnung von Betreuer/Institut
%
\newtoggle{printInstitutes}
\togglefalse{printInstitutes}
\renewcommand{\printinstitutes}{}
\newcommand{\instref}{}
\renewcommand{\addinstitute}[3]{%
	\expandafter\def\csname instref#1\endcsname{#3}%          % Definiere z.B. \instreflfbs als #3
	\expandafter\def\csname instname#1\endcsname{#2}%         % Definiere z.B. \instnamelfbs als #2
	% Neues Institut an "Tabelle" anhaengen
	\protected@edef\printinstitutes{\printinstitutes#3 #2\\}% % \printinstitutes expandieren und mit Zusatz
	                                                          %   wieder speichern
	\toggletrue{printInstitutes}%                       % Wenn wir ein Inst. definiert haben, auch drucken
}
% Anwenderbefehl zum referenzieren des Instituts
\NewDocumentCommand \instituteref { m }{                            % Definiere z.B. \instituteref{lfbs} als \instreflfbs
	\expandafter\csname instref#1\endcsname
%	\csdef{instref#1}{}
}


%
% Betreuer hinzufuegen
%
\newlength{\@templength}
\newlength{\@supervisorlength}                        % Laenge des laengsten Betreuernamens
\newlength{\@supervisorlabellength}                   % Laenge des laengsten Betreuerlabels (Betreuer, Zweitgutachter etc.)
\newlength{\@supervisornamelength}                    % Laenge des Betreuerlabels (z.B. Betreuer)
\newlength{\@secondrefereenamelength}                 % Laenge des Zweitgutachterlabels (z.B. Zweitgutachter)


\NewDocumentCommand \addsupervisor { O{} m }{%
	\ifcsundef{instref#1}{% \instrefxyz not defined 
  		\ClassError{acsthesis}
  		           {Institute "#1" not defined. Institute must be defined before using a reference to it}
  		           {Please define institute before using a reference to it.}
	}{%else
		\settowidth{\@templength}{#2 \instituteref{#1}}
		\ifnumcomp{\@templength}{>}{\@supervisorlength}{%
			\setlength{\@supervisorlength}{\@templength}
		}{%else
		}
		% Neuen Betreuer an "Tabelle" anhaengen
		\ifthenelse{\equal{\printsupervisors}{}}
			{\g@addto@macro\printsupervisors{#2 \instituteref{#1}}}    %1. Eintrag
			{\g@addto@macro\printsupervisors{\\ #2 \instituteref{#1}}} %Folgende Eintraege
	}
}

%
% Zweitgutachter hinzufuegen
%
\newtoggle{secondReferee}
\togglefalse{secondReferee}
\NewDocumentCommand \addsecondreferee { O{} m }{%
	\ifcsundef{instref#1}{% \instrefxyz not defined 
  		\ClassError{acsthesis}
  		           {Institute "#1" not defined. Institute must be defined before using a reference to it}
  		           {Please define institute before using a reference to it.}
	}{%else
		\settowidth{\@templength}{#2 \instituteref{#1}}
		\ifthenelse{\@templength>\@supervisorlength}
			{\setlength{\@supervisorlength}{\@templength}}%true
			{}%false
		% Neuen Zweitgutachter an "Tabelle" anhaengen
		\ifthenelse{\equal{\printsecondreferees}{}}
			{\g@addto@macro\printsecondreferees{#2 \instituteref{#1}}}    %1. Eintrag
			{\g@addto@macro\printsecondreferees{\\ #2 \instituteref{#1}}} %Folgende Eintraege
		\toggletrue{secondReferee}
	}
}


% ----- Statement of authorship
\NewDocumentCommand \statementOfAuthorship { } {%
	\thispagestyle{empty}

	\vspace*{\stretch{10}}
	
	\@statement
	\vspace{3cm}

	\@aachenDateName	
	\vspace{\stretch{1}}
}

% ----- Language dependent variables
\iflanguage{ngerman}{%
	\def\@tpName{Titelseite}
	\def\@tocName{Inhaltsverzeichnis}
	\def\@mainTitle{\@titleDE}
	\def\@subTitle{\@titleEN}
	\def\@matrNrName{Matrikelnummer}
	\def\@supervisorName{Betreuer}
	\def\@secondRefereeName{Zweitgutachter}
	\def\@aachenDateName{Aachen, den \@submissionDate}
	\def\@rwthAcsName{%
		an der\\
		Rheinisch-Westf\"alischen Technischen Hochschule Aachen\\
		Fakult\"at f\"ur Elektrotechnik und Informationstechnik\\
		Institute f\"ur Automation of Complex Power Systems\\
		Univ.\,Prof.\,Dr.-Ing. Antonello Monti
	}
	\def\@statement{%
		Ich versichere hiermit, dass ich die vorliegende Arbeit
		selbst\"andig und ohne Benutzung anderer als der
		angegebenen Hilfsmittel angefertigt habe. Alle Stellen,
		die w\"ortlich oder sinngem\"a\ss\ aus ver\"offentlichten und 
		nicht ver\"offentlichten Schriften entnommen sind, sind
		als solche kenntlich gemacht. Die Arbeit ist in gleicher
		oder \"ahnlicher Form noch nicht als Pr\"ufungsarbeit
		eingereicht worden.
	}
}{%
	\renewcommand\lstlistlistingname{List of Listings}
%
	\def\@tpName{Title Page}
	\def\@tocName{Table of Contents}
	\def\@mainTitle{\@titleEN}
	\def\@subTitle{\@titleDE}
	\def\@matrNrName{Matriculation Number}
	\def\@supervisorName{Supervisor}
	\def\@secondRefereeName{Second Referee}
	\def\@aachenDateName{Aachen, \@submissionDate}
	\def\@rwthAcsName{%
		at\\
		RWTH Aachen University\\
		Faculty of Electrical Engineering and Information Technology\\
		Institute for Automation of Complex Power Systems\\
		Univ.\,Prof.\,Dr.-Ing. Antonello Monti
	}
	\def\@statement{%
		I hereby declare that I wrote this thesis myself without
		sources other than those indicated herein. All parts taken
		from published and unpublished scripts are indicated as
		such. The paper has not been previously presented as an 
		examination paper in any other form.
	}
}

% ----- Define title page
\RenewDocumentCommand \maketitle { } {%
	\hypersetup{%
	            pdftitle={\@mainTitle},      % Titel, Autor und Subject in
	            pdfsubject={\@thesisType},   %  PDF-Eigenschaften eintragen
	            pdfauthor={\@author}
	}%
	\settowidth{\@supervisornamelength}{\@supervisorName}
	\settowidth{\@secondrefereenamelength}{\@secondRefereeName}
	\iftoggle{secondReferee}{%
		\ifnumcomp{\@supervisornamelength}{>}{\@secondrefereenamelength}{%
			\setlength{\@supervisorlabellength}{\@supervisornamelength}
		}{%else
			\setlength{\@supervisorlabellength}{\@secondrefereenamelength}
		}
	}{%else
		\setlength{\@supervisorlabellength}{\@supervisornamelength}
	}

	\begin{titlepage}
		\begin{tikzpicture}[remember picture, overlay]
			\ifthenelse{\boolean{outputPrintVersion}}{
				\node [anchor=north east] at ($(current page.north east)-(1cm, 1cm)$) {\includegraphics[width=.4\paperwidth]{logos/rwth_acs_combined}};
			}{
			}
		\end{tikzpicture}

		\begin{center}
			\phantomsection\pdfbookmark{\@tpName}{titel}
			\vspace{3cm plus 0cm minus 2.5cm}
%			\hfill\includegraphics[width=.4\paperwidth]{logos/rwth_acs_combined} \\ 
%			\vspace*{2.5cm plus 1cm minus 2cm}
			{\sffamily{\LARGE\bfseries\@mainTitle\par\vspace{1cm}}{\Large\bfseries\@subTitle\par}}

			\vspace{1cm}
			\vspace{\stretch{1}}
	    
			\@author\\
			\@matrNrName: \@matrNr\\
			\vspace{3cm plus 0cm minus 1.5cm}
			\textbf{\@thesisType}\\
			\@rwthAcsName
%			\vspace{0.8cm} \\
			\vspace{2.8cm plus 0cm minus 1cm}\\

			\begin{tikzpicture}
			 \node [] (supervisorLab) at (0,0) {\@supervisorName:};
			 \node [anchor=north west] (supervisors) at ($(supervisorLab.north west)+(\@supervisorlabellength, 0)+(1em, 0)$) {
				\begin{minipage}[t]{\@supervisorlength}%
					\printsupervisors
				\end{minipage}
			};
			\iftoggle{secondReferee}{%
				\coordinate (supervisorBtm) at ($(supervisorLab.north west)!(supervisors.south west)!(supervisorLab.south west)$);
				\node [anchor=north west] (sndRefereeLab) at (supervisorBtm) {\@secondRefereeName:};
				\node [anchor=north west] (supervisors) at ($(sndRefereeLab.north west)+(\@supervisorlabellength, 0)+(1em, 0)$) {
					\begin{minipage}[t]{\@supervisorlength}%
						\printsecondreferees
					\end{minipage}
			};
			}

			\end{tikzpicture}

			\vspace*{1cm}
			
			\ifthenelse{\boolean{outputPrintVersion}}{}{
				\textbf{Online PDF Version}
			}
			
			\iftoggle{printInstitutes}{%
				\printinstitutes
			}{%else
			}
		\end{center}
	\end{titlepage}
}

\NewDocumentCommand \appendixintro {} {%
	\chapter*{} 				% TODO: Can we omit this dirty hack?
	\thispagestyle{empty}
	{%
		\centering
		\vfill
		\Huge\bfseries\sffamily
		Appendix
		\vfill
	}
}


%------------------------------------------------------------------------------%
% Create cover, table of contents, etc. 
%------------------------------------------------------------------------------%
\AtBeginDocument{%
% Everything before the first chapter with roman page numbers
	\frontmatter
	\maketitle
	\cleardoublepage
	\statementOfAuthorship
	\cleardoublepage
	\phantomsection\pdfbookmark{\abstractname}{abstract}
	\input{abstract}
	\cleardoublepage
	\phantomsection\pdfbookmark{\@tocName}{toc}
	\tableofcontents

% Here comes the students code
\mainmatter
}

\AtEndDocument{%
	\printbibliography
	\printindex
}
