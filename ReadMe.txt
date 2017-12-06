This directory is the distribution of version 3.0 of the link grammar
parsing system for Microsoft Windows.  This version was created in
April, 1998.  (A Unix version is also available from this FTP site.)
Please see our web page http://www.link.cs.cmu.edu/link for more
information, including documentation.

CONTENTS of this directory:

   source/          This is a directory containing the source code of
                    the system, which is written in ANSI C.  See the
                    comments below for more information on building it.

   3.0.dict         The file containing the dictionary definitions.

   3.0.knowledge    The post-processing knowledge file that goes with
                    3.0.dict.

   tiny.dict        A small sample dictionary.

   words/*          Lists of words.

   3.0.batch        This is a batch file of sentences (both grammatical
                    and ungrammatical ones) that are handled correctly
                    by this release of the system.  Feed this into the
                    parser with "parse.exe 3.0.dict < 3.0.batch" as a
                    DOS command.

   parse.exe        An executable program.  Just double click
                    it to run the program in a DOS window.

CREATING the system:

   To build the system, you'll need Microsoft Developer Studio, Visual
   C++.  If you have this, double clicking on parse.dsw in the source
   directorywill fire up the the Developer Studio.  Then choose "rebuild
   all" in the "build" menu.

RUNNING the program:

   To run the program double click on parse.exe, or issue the DOS
   command "parse.exe" or "parse.exe 3.0.dict".

   Help is available there with "!help".  A number of user-settable
   variables control what happens.  "!var" shows these variables and
   their current values.

   We've used a special batch mode for testing the system on a large
   number of sentences.  The following DOS command runs the parser on
   3.0.batch:

       parse.exe 3.0.dict < 3.0.batch

   The line "!batch" near the top of 3.0.batch turns on batch mode.  In
   this mode sentences labeled with an initial "*" should be rejected
   and those not starting with a "*" should be accepted.  Any deviation
   from this behavior is reported as an error.

USING the parser in your own applications:

   There is a API (application program interface) to the parser.  This
   makes it easy to incorporate it into your own applications.  This is
   documented on our web site.

COMMERCIAL use:

   We feel that our system (or a derivative of it) could be useful in a
   number of commercial applications.  We would be pleased to discuss
   terms with those wanting to use our system in this way.  Without an
   explicit agreement with the authors (listed below) such use is
   forbidden.  By "commercial use" we mean selling a product or service.
   Of course everybody is free to take the system for purposes of
   experiment and evaluation.  Of course all academic products or papers
   must reference our work.

ADDRESSES

   If you have any questions, or find any bugs, please feel free to send
   a note to:

     John Lafferty                     lafferty@cs.cmu.edu
     Computer Science Department       412-268-6791
     Carnegie Mellon University        www.cs.cmu.edu/~lafferty
     Pittsburgh, PA 15213              

     Daniel Sleator                    sleator@cs.cmu.edu  
     Computer Science Department       412-268-7563
     Carnegie Mellon University        www.cs.cmu.edu/~sleator
     Pittsburgh, PA 15213              

     Davy Temperley	               dt3@columbia.edu
     Department of Music	       614-292-7321
     Weigel Hall                       www.columbia.edu/~dt3
     Ohio State University
     Columbus, OH 43201
