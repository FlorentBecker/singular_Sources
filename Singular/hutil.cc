/****************************************
*  Computer Algebra System SINGULAR     *
****************************************/
static char rcsid[] = "$Header: /exports/cvsroot-2/cvsroot/Singular/hutil.cc,v 1.1.1.1 1997-03-19 13:18:45 obachman Exp $";
/* $Log: not supported by cvs2svn $
*/

/*
* ABSTRACT: Utilities for staircase operations
*/

#include "mod2.h"
#include "tok.h"
#include "febase.h"
#include "mmemory.h"
#include "ipid.h"
#include "ideals.h"
#include "polys.h"
#include "hutil.h"

scfmon hexist, hstc, hrad, hwork;
scmon hpure, hpur0;
varset hvar, hsel;
int  hNexist, hNstc, hNrad, hNvar, hNpure;
short  hisModule;
monf stcmem, radmem;


scfmon hInit(ideal S, ideal Q, int *Nexist)
{
  int  sl, ql, i, k = 0;
  polyset si, qi, ss;
  scfmon ex, ek;
  hisModule = idRankFreeModule(S);
  if (hisModule < 0)
    hisModule = 0;
  if (S)
  {
    si = S->m;
    sl = IDELEMS(S);
  }
  else
  {
    si = NULL;
    sl = 0;
  }
  if (Q)
  {
    qi = Q->m;
    ql = IDELEMS(Q);
  }
  else
  {
    qi = NULL;
    ql = 0;
  }
  if (!(sl + ql))
  {
    *Nexist = 0;
    return NULL;
  }
  ss = si;
  for (i = sl; i; i--)
  {
    if (*ss)
      k++;
    ss++;
  }
  ss = qi;
  for (i = ql; i; i--)
  {
    if (*ss)
      k++;
    ss++;
  }
  *Nexist = k;
  if (!k)
    return NULL;
  ek = ex = (scfmon)Alloc(k * sizeof(scmon));
  for (i = sl; i; i--)
  {
    if (*si)
    {
      *ek = (scmon)((*si)->exp);
      ek++;
    }
    si++;
  }
  for (i = ql; i; i--)
  {
    if (*qi)
    {
      *ek = (scmon)((*qi)->exp);
      ek++;
    }
    qi++;
  }
  return ex;
}


void hComp(scfmon exist, int Nexist, short ak, scfmon stc, int *Nstc)
{
  int  i = Nexist, k = 0;
  scfmon ex = exist, co = stc;
  for (; i; i--)
  {
    if (!(**ex) || ((**ex) == ak))
    {
      *co = *ex;
      co++;
      k++;
    }
    ex++;
  }
  *Nstc = k;
}


void hSupp(scfmon stc, int Nstc, varset var, int *Nvar)
{
  int  nv, i0, i1, i, j;
  nv = i0 = *Nvar;
  i1 = 0;
  for (i = 1; i <= nv; i++)
  {
    j = 0;
    loop
    {
      if (stc[j][i])
      {
        i1++;
        var[i1] = i;
        break;
      }
      j++;
      if (j == Nstc)
      {
        var[i0] = i;
        i0--;
        break;
      }
    }
  }
  *Nvar = i1;
}


void hOrdSupp(scfmon stc, int Nstc, varset var, int Nvar)
{
  int  i, i1, j, jj, k, l;
  short  x;
  scmon temp;
  varset v1;
  v1 = (int *)Alloc(Nvar * sizeof(int));
  temp = (short *)Alloc(Nstc * sizeof(short));
  for (i = 1; i <= Nvar; i++)
  {
    i1 = var[i];
    *temp = stc[0][i1];
    jj = 1;
    for (j = 1; j < Nstc; j++)
    {
      x = stc[j][i1];
      k = 0;
      loop
      {
        if (x > temp[k])
        {
          k++;
          if (k == jj)
          {
            temp[k] = x;
            jj++;
            break;
          }
        }
        else if (x < temp[k])
        {
          for (l = jj; l < k; l--)
            temp[l] = temp[l-1];
          temp[k] = x;
          jj++;
          break;
        }
        else
          break;
      }
    }
    jj *= 2;
    if (!(*temp))
      jj--;
    v1[i-1] = jj;
  }
  for (i = 1; i < Nvar; i++)
  {
    i1 = var[i+1];
    jj = v1[i];
    j = 0;
    loop
    {
      if (jj > v1[j])
      {
        for (l = i; l > j; l--)
        {
          v1[l] = v1[l-1];
          var[l+1] = var[l];
        }
        v1[j] = jj;
        var[j+1] = i1;
        break;
      }
      j++;
      if (j == i)
        break;
    }
  }
  Free((ADDRESS)temp, Nstc * sizeof(short));
  Free((ADDRESS)v1, Nvar * sizeof(int));
}


static void hShrink(scfmon co, int a, int Nco)
{
  int  i = a, j = a;
  for (; j < Nco; j++)
  {
    if (co[j])
    {
      co[i] = co[j];
      i++;
    }
  }
}


void hStaircase(scfmon stc, int *Nstc, varset var, int Nvar)
{
  int  nc = *Nstc, z = 0, i, j, k, k1;
  scmon n, o;
  if (nc < 2)
    return;
  i = 0;
  j = 1;
  n = stc[j];
  o = *stc;
  k = Nvar;
  loop
  {
    k1 = var[k];
    if (o[k1] > n[k1])
    {
      loop
      {
        k--;
        if (!k)
        {
          stc[i] = NULL;
          z++;
          break;
        }
        else
        {
          k1 = var[k];
          if (o[k1] < n[k1])
            break;
        }
      }
      k = Nvar;
    }
    else if (o[k1] < n[k1])
    {
      loop
      {
        k--;
        if (!k)
        {
          stc[j] = NULL;
          z++;
          break;
        }
        else
        {
          k1 = var[k];
          if (o[k1] > n[k1])
            break;
        }
      }
      k = Nvar;
    }
    else
    {
      k--;
      if (!k)
      {
        stc[j] = NULL;
        z++;
        k = Nvar;
      }
    }
    if (k == Nvar)
    {
      if (!stc[j])
        i = j - 1;
      loop
      {
        i++;
        if (i == j)
        {
          i = -1;
          j++;
          if (j < nc)
            n = stc[j];
          else
          {
            if (z)
            {
              *Nstc -= z;
              hShrink(stc, 0, nc);
            }
            return;
          }
        }
        else if (stc[i])
        {
          o = stc[i];
          break;
        }
      }
    }
  }
}


void hRadical(scfmon rad, int *Nrad, int Nvar)
{
  int  nc = *Nrad, z = 0, i, j, k;
  scmon n, o;
  if (nc < 2)
    return;
  i = 0;
  j = 1;
  n = rad[j];
  o = *rad;
  k = Nvar;
  loop
  {
    if (o[k] && !n[k])
    {
      loop
      {
        k--;
        if (!k)
        {
          rad[i] = NULL;
          z++;
          break;
        }
        else
        {
          if (!o[k] && n[k])
            break;
        }
      }
      k = Nvar;
    }
    else if (!o[k] && n[k])
    {
      loop
      {
        k--;
        if (!k)
        {
          rad[j] = NULL;
          z++;
          break;
        }
        else
        {
          if (o[k] && !n[k])
            break;
        }
      }
      k = Nvar;
    }
    else
    {
      k--;
      if (!k)
      {
        rad[j] = NULL;
        z++;
        k = Nvar;
      }
    }
    if (k == Nvar)
    {
      if (!rad[j])
        i = j - 1;
      loop
      {
        i++;
        if (i == j)
        {
          i = -1;
          j++;
          if (j < nc)
            n = rad[j];
          else
          {
            if (z)
            {
              *Nrad -= z;
              hShrink(rad, 0, nc);
            }
            return;
          }
        }
        else if (rad[i])
        {
          o = rad[i];
          break;
        }
      }
    }
  }
}


void hLexS(scfmon stc, int Nstc, varset var, int Nvar)
{
  int  j = 1, i = 0, k, k1;
  scmon n, o;
  if (Nstc < 2)
    return;
  n = stc[j];
  o = *stc;
  k = Nvar;
  loop
  {
    k1 = var[k];
    if (o[k1] < n[k1])
    {
      i++;
      if (i < j)
      {
        o = stc[i];
        k = Nvar;
      }
      else
      {
        j++;
        if (j < Nstc)
        {
          i = 0;
          o = *stc;
          n = stc[j];
          k = Nvar;
        }
        else
          return;
      }
    }
    else if (o[k1] > n[k1])
    {
      for (k = j; k > i; k--)
        stc[k] = stc[k - 1];
      stc[i] = n;
      j++;
      if (j < Nstc)
      {
        i = 0;
        o = *stc;
        n = stc[j];
        k = Nvar;
      }
      else
        return;
    }
    else
      k--;
  }
}


void hLexR(scfmon rad, int Nrad, varset var, int Nvar)
{
  int  j = 1, i = 0, k, k1;
  scmon n, o;
  if (Nrad < 2)
    return;
  n = rad[j];
  o = *rad;
  k = Nvar;
  loop
  {
    k1 = var[k];
    if (!o[k1] && n[k1])
    {
      i++;
      if (i < j)
      {
        o = rad[i];
        k = Nvar;
      }
      else
      {
        j++;
        if (j < Nrad)
        {
          i = 0;
          o = *rad;
          n = rad[j];
          k = Nvar;
        }
        else
          return;
      }
    }
    else if (o[k1] && !n[k1])
    {
      for (k = j; k > i; k--)
        rad[k] = rad[k - 1];
      rad[i] = n;
      j++;
      if (j < Nrad)
      {
        i = 0;
        o = *rad;
        n = rad[j];
        k = Nvar;
      }
      else
        return;
    }
    else
      k--;
  }
}


void hPure(scfmon stc, int a, int *Nstc, varset var, int Nvar,
 scmon pure, int *Npure)
{
  int  nc = *Nstc, np = 0, nq = 0, j, i, i1, c, l;
  scmon x;
  for (j = a; j < nc; j++)
  {
    x = stc[j];
    i = Nvar;
    c = 2;
    l = 0;
    loop
    {
      i1 = var[i];
      if (x[i1])
      {
        c--;
        if (!c)
        {
          l = 0;
          break;
        }
        else if (c == 1)
          l = i1;
      }
      i--;
      if (!i)
        break;
    }
    if (l)
    {
      if (!pure[l])
      {
        np++;
        pure[l] = x[l];
      }
      else if (x[l] < pure[l])
        pure[l] = x[l];
      stc[j] = NULL;
      nq++;
    }
  }
  *Npure = np;
  if (nq)
  {
    *Nstc -= nq;
    hShrink(stc, a, nc);
  }
}


void hElimS(scfmon stc, int *e1, int a2, int e2, varset var, int Nvar)
{
  int  nc = *e1, z = 0, i, j, k, k1;
  scmon n, o;
  if (!nc || (a2 == e2))
    return;
  j = 0;
  i = a2;
  o = stc[i];
  n = *stc;
  k = Nvar;
  loop
  {
    k1 = var[k];
    if (o[k1] > n[k1])
    {
      k = Nvar;
      i++;
      if (i < e2)
        o = stc[i];
      else {
        j++;
        if (j < nc)
        {
          i = a2;
          o = stc[i];
          n = stc[j];
        }
        else
        {
          if (z)
          {
            *e1 -= z;
            hShrink(stc, 0, nc);
          }
          return;
        }
      }
    }
    else
    {
      k--;
      if (!k)
      {
        stc[j] = NULL;
        z++;
        j++;
        if (j < nc)
        {
          i = a2;
          o = stc[i];
          n = stc[j];
          k = Nvar;
        }
        else
        {
          if (z)
          {
            *e1 -= z;
            hShrink(stc, 0, nc);
          }
          return;
        }
      }
    }
  }
}


void hElimR(scfmon rad, int *e1, int a2, int e2, varset var, int Nvar)
{
  int  nc = *e1, z = 0, i, j, k, k1;
  scmon n, o;
  if (!nc || (a2 == e2))
    return;
  j = 0;
  i = a2;
  o = rad[i];
  n = *rad;
  k = Nvar;
  loop
  {
    k1 = var[k];
    if (o[k1] && !n[k1])
    {
      k = Nvar;
      i++;
      if (i < e2)
        o = rad[i];
      else
      {
        j++;
        if (j < nc)
        {
          i = a2;
          o = rad[i];
          n = rad[j];
        }
        else
        {
          if (z)
          {
            *e1 -= z;
            hShrink(rad, 0, nc);
          }
          return;
        }
      }
    }
    else
    {
      k--;
      if (!k)
      {
        rad[j] = NULL;
        z++;
        j++;
        if (j < nc)
        {
          i = a2;
          o = rad[i];
          n = rad[j];
          k = Nvar;
        }
        else
        {
          if (z)
          {
            *e1 -= z;
            hShrink(rad, 0, nc);
          }
          return;
        }
      }
    }
  }
}


void hLex2S(scfmon rad, int e1, int a2, int e2, varset var,
 int Nvar, scfmon w)
{
  int  j0 = 0, j = 0, i = a2, k, k1;
  scmon n, o;
  if (!e1)
  {
    for (; i < e2; i++)
      rad[i - a2] = rad[i];
    return;
  }  else if (i == e2)
    return;
  n = rad[j];
  o = rad[i];
  loop
  {
    k = Nvar;
    loop
    {
      k1 = var[k];
      if (o[k1] < n[k1])
      {
        w[j0] = o;
        j0++;
        i++;
        if (i < e2)
        {
          o = rad[i];
          break;
        }
        else
        {
          for (; j < e1; j++)
          {
            w[j0] = rad[j];
            j0++;
          }
          memcpy(rad, w, (e1 + e2 - a2) * sizeof(scmon));
          return;
        }
      }
      else if (o[k1] > n[k1])
      {
        w[j0] = n;
        j0++;
        j++;
        if (j < e1)
        {
          n = rad[j];
          break;
        }
        else
        {
          for (; i < e2; i++)
          {
            w[j0] = rad[i];
            j0++;
          }
          memcpy(rad, w, (e1 + e2 - a2) * sizeof(scmon));
          return;
        }
      }
      k--;
    }
  }
}


void hLex2R(scfmon rad, int e1, int a2, int e2, varset var,
 int Nvar, scfmon w)
{
  int  j0 = 0, j = 0, i = a2, k, k1;
  scmon n, o;
  if (!e1)
  {
    for (; i < e2; i++)
      rad[i - a2] = rad[i];
    return;
  }
  else if (i == e2)
    return;
  n = rad[j];
  o = rad[i];
  loop
  {
    k = Nvar;
    loop
    {
      k1 = var[k];
      if (!o[k1] && n[k1])
      {
        w[j0] = o;
        j0++;
        i++;
        if (i < e2)
        {
          o = rad[i];
          break;
        }
        else
        {
          for (; j < e1; j++)
          {
            w[j0] = rad[j];
            j0++;
          }
          memcpy(rad, w, (e1 + e2 - a2) * sizeof(scmon));
          return;
        }
      }
      else if (o[k1] && !n[k1])
      {
        w[j0] = n;
        j0++;
        j++;
        if (j < e1)
        {
          n = rad[j];
          break;
        }
        else
        {
          for (; i < e2; i++)
          {
            w[j0] = rad[i];
            j0++;
          }
          memcpy(rad, w, (e1 + e2 - a2) * sizeof(scmon));
          return;
        }
      }
      k--;
    }
  }
}


void hStepS(scfmon stc, int Nstc, varset var, int Nvar, int *a, short *x)
{
  int  k1, i;
  short  y;
  k1 = var[Nvar];
  y = *x;
  i = *a;
  loop
  {
    if (y < stc[i][k1])
    {
      *a = i;
      *x = stc[i][k1];
      return;
    }
    i++;
    if (i == Nstc)
    {
      *a = i;
      return;
    }
  }
}


void hStepR(scfmon rad, int Nrad, varset var, int Nvar, int *a)
{
  int  k1, i;
  k1 = var[Nvar];
  i = 0;
  loop
  {
    if (rad[i][k1])
    {
      *a = i;
      return;
    }
    i++;
    if (i == Nrad)
    {
      *a = i;
      return;
    }
  }
}


monf hCreate(int Nvar)
{
  monf xmem;
  int  i;
  xmem = (monf)Alloc((Nvar + 1) * sizeof(monp));
  for (i = Nvar; i; i--)
  {
    xmem[i] = (monp)Alloc(LEN_MON);
    xmem[i]->mo = NULL;
  }
  return xmem;
}


void hKill(monf xmem, int Nvar)
{
  int  i;
  for (i = Nvar; i; i--)
  {
    if (xmem[i]->mo)
      Free((ADDRESS)xmem[i]->mo, xmem[i]->a * sizeof(scmon));
    Free((ADDRESS)xmem[i], LEN_MON);
  }
  Free((ADDRESS)xmem, (Nvar + 1) * sizeof(monp));
}


scfmon hGetmem(int lm, scfmon old, monp monmem)
{
  scfmon x = monmem->mo;
  int  lx = monmem->a;
  if (!x || (lm > lx))
  {
    Free((ADDRESS)x, lx * sizeof(scmon));
    monmem->mo = x = (scfmon)Alloc(lm * sizeof(scmon));
    monmem->a = lm;
  }
  memcpy(x, old, lm * sizeof(scmon));
  return x;
}


scmon hGetpure(scmon p)
{
  scmon p1, pn;
  p1 = p + 1;
  pn = p1 + pVariables;
  memcpy(pn, p1, pVariables * sizeof(short));
  return pn - 1;
}


