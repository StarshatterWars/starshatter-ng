FORM

// +--------------------------------------------------------------------+
//  Project:   Starshatter 5.0
//  File:      CmpSceneDlg.frm
//
//  Destroyer Studios LLC
//  Copyright © 1997-2007. All Rights Reserved.
// +--------------------------------------------------------------------+

form: {
   back_color: (0,0,0)

   layout: {
      x_mins:     (0,0,0)
      x_weights:  (1,9,1)

      y_mins:     (0, 200, 0,  28, 0  )
      y_weights:  (1,   6, 0.3, 0, 0.5)
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
   }

   ctrl: {
      id:            101
      type:          label
      cells:         (0,1,3,1)
      hide_partial:  false
      fore_color:       (0,0,0)
      back_color:       (0,0,0)
      cell_insets:      (0,0,0,0)
   }

   ctrl: {
      id:               102
      type:             text
      text:             ""
      scroll_bar:       0
      smooth_scroll:    false
      transparent:      true
      align:            left
      cells:            (1,3,1,1)
      hide_partial:     false
      fore_color:       (255,255,128)
      back_color:       (0,0,0)
      cell_insets:      (0,0,0,0)
   }
}
