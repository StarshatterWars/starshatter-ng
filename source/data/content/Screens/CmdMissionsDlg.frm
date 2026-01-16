FORM

// +--------------------------------------------------------------------+
//  Project:   Starshatter 5.0
//  File:      CmdMissionsDlg.frm
//
//  Destroyer Studios LLC
//  Copyright © 1997-2007. All Rights Reserved.
// +--------------------------------------------------------------------+

form: {
   back_color: (  0,   0,   0)
   fore_color: (255, 255, 255)

   texture:    "Frame1.pcx"
   margins:    (1,1,64,8)

   layout: {
      x_mins:     (10, 100,  20, 100, 100, 100, 10)
      x_weights:  ( 0,   1,   0,   2,   2,   2,  0)

      y_mins:     (28, 30,  10,  90, 24, 60, 45)
      y_weights:  ( 0,  0,   0,   0,  0,  1,  0)
   }

   // background images:

   ctrl: {
      id:            9990
      type:          background
      texture:       Frame4a
      cells:         (1,3,5,1),
      cell_insets:   (0,0,0,0),
      margins:       (2,2,16,16)
      hide_partial:  false
   }

   ctrl: {
      id:            9991,
      type:          background,
      texture:       Frame2a,
      cells:         (1,4,3,3),
      cell_insets:   (0,0,0,10),
      margins:       (2,32,40,32)
      hide_partial:  false
   }

   ctrl: {
      id:            9992,
      type:          background,
      texture:       Frame2b,
      cells:         (4,4,2,3),
      cell_insets:   (0,0,0,10),
      margins:       (0,40,40,32)
      hide_partial:  false
   }

   // title:

   ctrl: {
      id:            10,
      type:          label,
      text:          "form.cmd.title",
      align:         left,
      font:          Limerick18,
      fore_color:    (255,255,255),
      transparent:   true,
      cells:         (1,1,3,1)
      cell_insets:   (0,0,0,0)
      hide_partial:  false
   },


   // info panel:

   ctrl: {
      id:               700
      type:             panel
      transparent:      false

      texture:          Panel
      margins:          (12,12,12,0),

      cells:            (1,3,5,1),
      cell_insets:      (10,10,12,10)

      layout: {
         x_mins:     ( 20, 100, 20, 100, 20)
         x_weights:  (  0,   1,  0,   1,  0)

         y_mins:     (  0,  20,  5,  20,  0)
         y_weights:  (  1,   0,  0,   0,  1)
      }
   }

   defctrl: {
      align:            left
      bevel_width:      0
      font:             Verdana
      fore_color:       (255, 255, 255)
      standard_image:   ""
      activated_image:  ""
      transparent:      true
      margins:          (0,0,0,0)
      cell_insets:      (0,0,0,0)
      text_insets:      (1,1,1,1)
      pid:              700
   },

   ctrl: {
      id:            300,
      type:          label,
      cells:         (1,1,1,1)
      text:          " ", // Operation Title Goes Here
      font:             Limerick12
      fore_color:    (255, 255, 128)
   },

   ctrl: {
      id:            301,
      type:          label,
      cells:         (3,1,1,1)
      text:          " ", // timestamp goes here
      align:         right
   },

   ctrl: {
      id:            200,
      type:          label,
      cells:         (1,3,1,1)
      text:          " ", // player group goes here
   },

   ctrl: {
      id:            201,
      type:          label,
      cells:         (3,3,1,1)
      align:         right,
   },

   // tab buttons:

   ctrl: {
      id:               600
      pid:              0
      type:             panel
      transparent:      true

      cells:            (1,5,1,1),
      cell_insets:      (10,10,12,54)

      layout: {
         x_mins:     ( 10)
         x_weights:  (  1)

         y_mins:     ( 25, 25, 25, 25, 25, 25)
         y_weights:  (  0,  0,  0,  0,  0,  1)
      }
   }


   defctrl: {
      align:            left
      font:             Limerick12
      fore_color:       (0,0,0)
      standard_image:   Button17_0
      activated_image:  Button17_1
      transition_image: Button17_2
      transparent:      false
      bevel_width:      6
      margins:          (3,18,0,0)
      cell_insets:      (0,0,0,0)
      pid:              600
      fixed_height:     19
   }

   ctrl: {
      id:            100
      type:          button
      text:          "form.cmd.orders"
      cells:         (0,0,1,1)
   }

   ctrl: {
      id:            101
      type:          button
      text:          "form.cmd.theater"
      cells:         (0,1,1,1)
   }

   ctrl: {
      id:            102
      type:          button
      text:          "form.cmd.forces"
      cells:         (0,2,1,1)
   }

   ctrl: {
      id:            103
      type:          button
      text:          "form.cmd.intel"
      cells:         (0,3,1,1)
   }

   ctrl: {
      id:            104
      type:          button
      text:          "form.cmd.missions"
      cells:         (0,4,1,1)
   }


   // main panel:

   ctrl: {
      id:               900
      pid:              0
      type:             panel
      transparent:      true
      fixed_height:     0

      cells:            (3,5,3,1)
      cell_insets:      (10,10,12,10)

      layout: {
         x_mins:     ( 10)
         x_weights:  (  1)

         y_mins:     ( 20, 100, 20. 100, 25)
         y_weights:  (  0,   1,  0,   1,  1)
      }
   }

   defctrl: {
      align:            left
      font:             Verdana
      back_color:       ( 41, 41, 41)
      fore_color:       (255,255,255)
      standard_image:   ""
      activated_image:  ""
      transition_image: ""
      transparent:      false

      texture:          Panel
      margins:          (12,12,12,0),

      bevel_width:      6
      cell_insets:      (0,0,0,10)
      pid:              900
      fixed_height:     0
   }

   ctrl: {
      id:            441
      type:          label
      text:          "form.cmd.mission.roster"
      cells:         (0,0,1,1)
      cell_insets:   (0,0,0,0)
      font:          Limerick12
      back_color:    (0,0,0)
      fore_color:    (255,255,255)
      style:         0x0040
      transparent:   true
   }

   ctrl: {
      id:            401
      type:          list
      cells:         (0,1,1,1)

      style:         0x02
      scroll_bar:    2
      show_headings: true

      column: { title: "form.cmd.mission.mission", width: 150, align: left,  sort: 0 }
      column: { title: "form.cmd.mission.type",    width: 150, align: left,  sort: 0 }
      column: { title: "form.cmd.mission.start",   width: 130, align: right, sort: 0 }
   },

   ctrl: {
      id:            442
      type:          label
      text:          "form.cmd.mission.objectives"
      cells:         (0,2,1,1)
      cell_insets:   (0,0,0,0)
      font:          Limerick12
      back_color:    (0,0,0)
      fore_color:    (255,255,255)
      style:         0x0040
      transparent:   true
   }

   ctrl: {
      id:            402
      type:          text
      cells:         (0,3,1,1)
      style:         0x02
      scroll_bar:    2
   },

   ctrl: {
      id:            403,
      type:          button,
      text:          "form.accept",
      cells:         (0,4,1,1)

      align:            left
      font:             Limerick12
      fore_color:       (0,0,0)
      standard_image:   Button17_0
      activated_image:  Button17_1
      transition_image: Button17_2
      transparent:      false
      bevel_width:      6
      margins:          (3,18,0,0)
      fixed_height:     19
      fixed_width:      150
   },


   // ok and cancel buttons:

   defctrl: {
      align:            left
      font:             Limerick12
      fore_color:       (0,0,0)
      standard_image:   Button17_0
      activated_image:  Button17_1
      transition_image: Button17_2
      transparent:      false
      bevel_width:      6
      margins:          (3,18,0,0)
      cell_insets:      (0,10,0,0)
      pid:              0
      fixed_height:     19
   },

   ctrl: {
      id:            1
      type:          button
      text:          "form.save"
      cells:         (4,6,1,1),
   }

   ctrl: {
      id:            2
      type:          button
      text:          "form.exit"
      cells:         (5,6,1,1),
   }
}
