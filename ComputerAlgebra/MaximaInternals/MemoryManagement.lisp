;; Oliver Kullmann, 15.1.2010 (Swansea)
;; Copyright 2010 Oliver Kullmann
;; This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
;; it and/or modify it under the terms of the GNU General Public License as published by
;; the Free Software Foundation and included in this library; either version 3 of the
;; License, or any later version.

;;!
;;  \file ComputerAlgebra/MaximaInternals/MemoryManagement.lisp
;;  \brief Setting and reading memory limits

;; Use by

;; oklib_load("OKlib/ComputerAlgebra/MaximaInternals/MemoryManagement.lisp");


(defun $set_frame_stack_ecl (fs)
  (ext:set-limit 'ext:frame-stack fs)
  fs
)
(defun $get_frame_stack_ecl ()
  (ext:get-limit 'ext:frame-stack)
)

(defun $set_binding_stack_ecl (bs)
  (ext:set-limit 'ext:binding-stack bs)
  bs
)
(defun $get_binding_stack_ecl ()
  (ext:get-limit 'ext:binding-stack)
)

(defun $set_c_stack_ecl (cs)
  (ext:set-limit 'ext:c-stack cs)
  cs
)
(defun $get_c_stack_ecl ()
  (ext:get-limit 'ext:c-stack)
)

(defun $set_heap_size_ecl (hs)
  (ext:set-limit 'ext:heap-size hs)
  hs
)
(defun $get_heap_size_ecl ()
  (ext:get-limit 'ext:heap-size)
)

(defun $set_lisp_stack_ecl (ls)
  (ext:set-limit 'ext:lisp-stack ls)
  ls
)
(defun $get_lisp_stack_ecl ()
  (ext:get-limit 'ext:lisp-stack)
)

