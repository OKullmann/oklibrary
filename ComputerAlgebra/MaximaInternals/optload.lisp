/* Copyright 2008 Andrej Vodopivec
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

(defvar *add-lineinfop* t)

(defun add-lineinfo (lis)
  (if *add-lineinfop*
      (if (or (atom lis) (and (eq *parse-window* *standard-input*)
			      (not (find-stream *parse-stream*))))
	  lis
	  (let* ((st (get-instream *parse-stream*))
		 (n (instream-line st))
		 (nam (instream-name st)))
	    (or nam (return-from add-lineinfo lis))
	    (setq *current-line-info*
		  (cond ((eq (cadr *current-line-info*) nam)
			 (cond ((eql (car *current-line-info*) n)
				*current-line-info*)
			       (t  (cons n (cdr *current-line-info*)))))
			(t (list n nam  'src))))
	    (cond ((null (cdr lis))
		   (list (car lis) *current-line-info*))
		  (t (append lis (list *current-line-info*))))))
      lis))


(defmfun $load (filename &optional (add-lineinfo t))
  "This is the generic file loading function.
  LOAD(filename) will either BATCHLOAD or LOADFILE the file,
  depending on wether the file contains Macsyma, Lisp, or Compiled
  code. The file specifications default such that a compiled file
  is searched for first, then a lisp file, and finally a macsyma batch
  file. This command is designed to provide maximum utility and
  convenience for writers of packages and users of the macsyma->lisp
  translator."

  (let ((searched-for
	 ($file_search1 filename
			'((mlist) $file_search_maxima $file_search_lisp  )))
	(*add-lineinfop* add-lineinfo)
	type)
    (setq type ($file_type searched-for))
    (case type
      (($maxima)
       ($batchload searched-for))
      (($lisp $object)
       ;; do something about handling errors
       ;; during loading. Foobar fail act errors.
       (load-and-tell searched-for))
      (t
       (merror "Maxima bug: Unknown file type ~M" type)))
    searched-for))
