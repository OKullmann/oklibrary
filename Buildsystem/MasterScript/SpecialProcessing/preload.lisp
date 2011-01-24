(format t ";;; Frame stack limit ~D"
        (ext:set-limit 'ext:frame-stack m4_SHELL(frame_stack_ecl_okl)))
(format t "~%;;; Binding stack limit ~D"
        (ext:set-limit 'ext:binding-stack m4_SHELL(binding_stack_ecl_okl)))
(format t "~%;;; C stack limit ~D"
        (ext:set-limit 'ext:c-stack m4_SHELL(c_stack_ecl_okl)))
(format t "~%;;; Heap size limit ~D"
        (ext:set-limit 'ext:heap-size m4_SHELL(heap_size_ecl_okl)))
(format t "~%;;; Lisp stack limit ~D~%;;;"
        (ext:set-limit 'ext:lisp-stack m4_SHELL(lisp_stack_ecl_okl)))
