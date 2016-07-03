/*  NETMAJ-XUI -- X User Interface --  Copyright Notice
 *
 *      Copyright (c) 1995,1996 Todo Software All rights reserved.
 *
 *   You can redistribute it and/or modify this software under the terms
 *  of the GNU General Public License version 2 as published by
 *  the Free Software Foundation.
 *
 *   This software is provided 'as-is', without any express or implied
 *  warranty.  In no event will the author be held liable for any damages
 *  arising from the use of this software.
 */

#include "xnet.h"
#include "pixread.h"
#include "xnetmaj.h"
#include "ahand.h"

#define MAN5R  (5 * 4)
#define PIN5R  (14 * 4)
#define SOU5R  (23 * 4)

int
whatres (global_t *gp)
{
  result_t y;
  ahand_t a;
  int ret = CAN_CANCEL;

  if (!flag_request)
    {
      return ret |= (CAN_RON | CAN_KAN | CAN_PON | CAN_TIE);
    }

  hand_insert (gp, vself, rvp->out);
  ah_new (&a, &hand[vself], -1);
  ah_analize (&a, 0);
  if (result_rest == 0 && (result_calc_yaku (gp, vself, &y) > 0))
    {
      ret |= CAN_RON;
    }
  hand_delete (gp, vself, rvp->out, 0);
  if (hand_can_kan (gp, vself, rvp->out) == 2)
    {
      ret |= CAN_KAN;
    }
  if (hand_can_pon (gp, vself, rvp->out))
    {
      ret |= CAN_PON;
    }
  if ((cur_player + 1) % 4 == vself && hand_can_tiex (gp, vself, rvp->out))
    {
      ret |= CAN_TIE;
    }
  return ret;
}

int
whatchoice (global_t *gp)
{
  result_t y;
  ahand_t a;
  int ret = CAN_CANCEL;
  int i;

  if (!flag_request)
    {
      return ret |= (CAN_TUMO | CAN_REACH | CAN_KAN);
    }
  ah_new (&a, &hand[vself], -1);
  ah_analize (&a, 0);
  if (result_rest == 0
      && (!hand[vself].naki || result_calc_yaku (gp, vself, &y) > 0))
    {
      ret |= CAN_TUMO;
    }
  if (result_check_kyushukyupai (gp))
    {
      ret |= CAN_TUMO;
    }
  if (!hand[vself].reach
      && !hand[vself].naki
      && (result_rest == 1 || result_rest == 0) && ask_attr != T_RV_RCH)
    {
      ret |= CAN_REACH;
    }
  {
    char pais[36];
    memset (pais, 0, sizeof (pais));
    for (i = 0; i < 4; i++)
      {
        if (hand[vself].opened_kind[i] == H_PON)
          {
            pais[P_KIND (hand[vself].opened[i][0])] += 3;
          }
      }
    for (i = 0; i < hand[vself].closed_num; i++)
      {
        pais[P_KIND (hand[vself].closed[i])]++;
        if (pais[P_KIND (hand[vself].closed[i])] == 4)
          {
            ret |= CAN_KAN;
            break;
          }
      }
  }

  return ret;
}


extern int auto_stop;
int in_res;
int in_choice;

int
ui_res (global_t *gp, int *howp)
{
  int flag;
  snd (audioplay_res ());
  flag = _ui_res (gp, howp);
  snd (audioplay_resend ());
  return flag;
}

int
_ui_res (global_t *gp, int *howp)
{
  int c, i, j, n, ret, pai;
  gpixmap2_t args[25];
  int pais[25];
  extern integer_t iam;

retry:
  *howp = 0;
  c = 0;

  iam = mytblpos (vself);

  if (auto_stop)
    {
      result_t y;
      if (!hand[vself].reach
          && (T_GET (rvp->attr) != T_HN_KAN)
          && (T_GET (rvp->attr) != T_HN_KAN_C))
        {
          if (hand_can_pon (gp, vself, rvp->out)
              || (hand_can_kan (gp, vself, rvp->out) == 2)
              || ((cur_player + 1) % 4 == vself
                  && hand_can_tiex (gp, vself, rvp->out)))
            c = ' ';
        }
      if (c == 0)
        {
          hand_insert (gp, vself, rvp->out);
          if (result_calc_yaku (gp, vself, &y) > 0)
            c = ' ';
          hand_delete (gp, vself, rvp->out, 0);
        }
    }

  if (c == 0 || c != ' ')
    {
      extern int play_speed;
      xsleep (play_speed);
      return R_ACK;
    }

  in_res = 1;
  ret = choice (gp, 0, 0, args, CH_TIE, whatres (gp));
  in_res = 0;
  switch (ret)
    {
    case X_CANCEL:
      return R_ACK;
    case X_TIE:
      break;
    case X_PON:
      return R_PON;
    case X_KAN:
      return R_KAN;
    case X_RON:
      return R_RON;
    }
  if ((cur_player + 1) % 4 != vself)
    goto retry;

  n = hand[vself].closed_num;
  i = 0;
  for (j = 0; j < n; j++)
    {
      pai = hand[vself].closed[j];
      if (i != 0 && P_KIND (pai) == P_KIND (pais[i - 1]))
        {
          if (pais[i - 1] != MAN5R
              && pais[i - 1] != PIN5R && pais[i - 1] != SOU5R)
            {
              continue;
            }
        }
      if (hand_can_tie (gp, vself, rvp->out, pai))
        {
          pais[i] = pai;
          args[i] = pai2pix (pai, 3, 0, 0);
          i++;
        }
    }
  if (i == 0)
    goto retry;
  if (i == 1)
    {
      *howp = pais[0];
      return R_TIE;
    }
  in_res = 1;
  ret = choice (gp, i, i, args, CH_TIE, 0);
  in_res = 0;
  iam = -1;

  if (ret == X_CANCEL)
    goto retry;
  if (ret >= i)
    {
      goto retry;
    }
  *howp = pais[ret];
  return R_TIE;
}

int
ui_choice (global_t *gp)
{
  int flag;
  snd (audioplay_choice ());
  flag = _ui_choice (gp);
  snd (audioplay_choiceend ());
  return flag;
}

int
_ui_choice (global_t *gp)
{
  int i, j, n, ret, pai, oldpai, kan;
  gpixmap2_t args[25];
  int pais[25];
  int mode = CH_NORM;
  int who, pos;
  widget_t canvas;

  pos = mytblpos (vself);
  mark (mark_frame[pos], linepos_frame[pos]);

loop:
  n = hand[vself].closed_num;
  in_choice = 1;
  ret = choice (gp, 0, n, args, mode, whatchoice (gp) | 0x80000000);
  in_choice = 0;
  if (ret < n)
    {
      ask_pai = hand[vself].closed[ret];
    }
  else if (ret == X_REACH)
    {
      mode = (mode == 1) ? CH_NORM : 1;
      ask_attr = (mode == 1) ? T_RV_RCH : T_RV;
      goto loop;
    }
  else if (ret == X_KAN)
    {
      oldpai = -1;
      n = hand[vself].closed_num;
      i = 0;
      for (j = 0; j < n; j++)
        {
          pai = hand[vself].closed[j];
          kan = hand_can_kan (gp, vself, pai);
          if ((kan == 3 || kan == 1) && P_KIND (pai) != oldpai)
            {
              pais[i] = pai;
              args[i] = pai2pix (pai, 3, 0, 0);
              i++;
              oldpai = P_KIND (pai);
            }
        }
      if (i == 0)
        goto loop;
      if (i == 1)
        {
          ask_attr = T_HN_KAN;
          ask_pai = pais[0];
          return;
        }
      ret = choice (gp, i, i, args, CH_TIE, 0);
      if (ret == X_CANCEL)
        {
          goto loop;
        }
      if (ret >= i)
        {
          goto loop;
        }
      ask_pai = pais[ret];
      ask_attr = T_HN_KAN;
      return;
    }
  else if (ret == X_TUMO)
    {
      ask_attr = T_HN_TSM;
    }
  else
    {
      goto loop;
    }
}
