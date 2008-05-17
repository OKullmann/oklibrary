;; Oliver Kullmann, 13.5.2008 (Guangzhou)
;; Copyright 2008 Oliver Kullmann
;; This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
;; it and/or modify it under the terms of the GNU General Public License as published by
;; the Free Software Foundation and included in this library; either version 3 of the
;; License, or any later version.

;;!
;;  \file ComputerAlgebra/DataStructures/Lisp/HashMaps.lisp
;;  \brief The internal basic Lisp functions for the hash table

;; Use by

;; oklib_load("Transitional/ComputerAlgebra/DataStructures/Lisp/HashMaps.lisp");


;;;;;;;
;;
;; Expose lisp hashtable
;; (from Maxima/5.15.0/share/maxima/5.15.0/share/contrib/graphs/graph_core.lisp)
;;;;;;

(defun $hash_table_okl ()
  (make-hash-table :test #'equal))

(defun $get_hash_okl (elt ht &optional default)
  (unless (hash-table-p ht)
    ($error "Second argument to `get_hash_okl' is not a hash table!"))
  (gethash elt ht default))

(defun $set_hash_okl (elt ht value)
  (unless (hash-table-p ht)
    ($error "Second argument to `set_hash_okl' is not a hash table!"))
  (setf (gethash elt ht) value)
  value)

(defun $hash_table_data_okl (ht)
  (unless (hash-table-p ht)
    ($error "First argument to `hash_table_data_okl' is not a hash table!"))
  (let (res)
    (maphash
     (lambda (key val)
       (setq res (cons `((marrow simp) ,key ,val) res)))
     ht)
    (cons '(mlist simp) res)))

(defun $del_hash_okl (elt ht)
  (unless (hash-table-p ht)
    ($error "Second argument to `del_hash_okl' is not a hash table!"))
  (remhash elt ht))

