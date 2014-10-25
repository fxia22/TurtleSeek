
(cl:in-package :asdf)

(defsystem "people_msgs-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "PositionMeasurement" :depends-on ("_package_PositionMeasurement"))
    (:file "_package_PositionMeasurement" :depends-on ("_package"))
  ))