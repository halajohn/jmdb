(require 'comint)
(require 'gud)

(defvar gud-jmdb-marker-regexp
  ;; The jmdb <--> emacs string format is:
  ;;
  ;; /aa/bb/cc/dd/ee.java:20\n
  ;;
  ;; The first 2 characters have to be , and
  ;; between  and the colon (:) is the source file name,
  ;; and use \( ... \) notation to mention it as subexpression 1.
  ;;
  ;; After the colon (:) is the line number,
  ;; and use \( ... \) notation to mention it as subexpression 2.
  ;; and the last will be a new line character.
  "\032\032\\(.[^:\n]*\\):\\([0-9]*\\).*\n")

(defun gud-jmdb-massage-args (file args)
  (if args
      (let (massaged-args user-error)
        
	(while
	    (and args
		 (not (string-match "--srcpath\\(.+\\)" (car args)))
		 (not (setq user-error
			    (string-match "--srcpath$" (car args)))))
	  (setq massaged-args (append massaged-args (list (car args))))
	  (setq args (cdr args)))
        
        (if user-error
            (progn
              (kill-buffer (current-buffer))
              (error "Error: Omit whitespace between '--srcpath' and its value")))
        
        (if args
            (setq massaged-args
                  (append
                   massaged-args
                   (list "--srcpath")
                   (list
                    (substring
                     (car args)
                     (match-beginning 1) (match-end 1)))
                   (cdr args)))
          massaged-args)
        
        (cons "--emacs" massaged-args))))

(defun gud-jmdb-marker-filter (string)
  (setq gud-marker-acc (concat gud-marker-acc string))
  
  (let ((output ""))
    (while (string-match gud-jmdb-marker-regexp gud-marker-acc)
      (setq gud-last-frame
            (cons (substring gud-marker-acc (match-beginning 1) (match-end 1))
                  (string-to-int (substring gud-marker-acc
                                            (match-beginning 2)
                                            (match-end 2))))
            output (concat output
                           (substring gud-marker-acc 0 (match-beginning 0)))
            
            gud-marker-acc (substring gud-marker-acc (match-end 0))))
    
    (if (string-match "\032.*\\'" gud-marker-acc)
        (progn
          (setq output (concat output (substring gud-marker-acc
                                                 0 (match-beginning 0))))
          
          (setq gud-marker-acc
                (substring gud-marker-acc (match-beginning 0))))
      
      (setq output (concat output gud-marker-acc)
            gud-marker-acc ""))
    
    output))

(defun gud-jmdb-find-file (f)
  (find-file-noselect f 'nowarn))

(defun jmdb (command-line)
  "Run jmdb"
  (interactive (list (gud-query-cmdline 'jmdb)))
  (gud-common-init command-line 'gud-jmdb-massage-args
                   'gud-jmdb-marker-filter 'gud-gdb-find-file)
  )
