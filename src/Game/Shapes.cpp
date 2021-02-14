#include "Shapes.hpp"

const b2Vec2 shipPoints[] = {
    b2Vec2(0.3, 0.5),
    b2Vec2(0, -0.49999999999999994),
    b2Vec2(-0.3, 0.5),
    b2Vec2(0.3, 0.5),
};
const int shipPointsCount = 4;

const b2Vec2 shipThrustPoints[] = {
    b2Vec2(0.3, 0.5),
    b2Vec2(0, -0.49999999999999994),
    b2Vec2(-0.3, 0.5),
    b2Vec2(0.3, 0.5),
    // some extra points for when we are thrusting
    b2Vec2(0.125, 0.7),
    b2Vec2(0.0, 0.5),
    b2Vec2(-0.125, 0.7),
    b2Vec2(-0.3, 0.5),
};
const int shipThrustPointsCount = 9;

const b2Vec2 bulletPoints[] = {
    b2Vec2(-0.025, 0.25),
    b2Vec2(0.025, 0.25),
    b2Vec2(0.025, -0.25),
    b2Vec2(-0.025, -0.25),
    b2Vec2(-0.025, 0.25)};
const int bulletPointsCount = 5;

const b2Vec2 asteroid1Points[] = {
    b2Vec2(0.057291626835217886, -0.4479166903502308),
    b2Vec2(0.1848958112645577, -0.24479167247990513),
    b2Vec2(0.4505208489429552, -0.255208337854741),
    b2Vec2(0.4088541736640834, -0.015624992894930764),
    b2Vec2(0.5, 0.19010418012323715),
    b2Vec2(0.22916671403379496, 0.23437501743971545),
    b2Vec2(0.0546875026913141, 0.4479166903502308),
    b2Vec2(-0.10156249515563455, 0.27604168927370515),
    b2Vec2(-0.4999999999999999, 0.12239584657459872),
    b2Vec2(-0.3124999956938973, -0.1380208325797654),
    b2Vec2(-0.23697915536314731, -0.38802083602464743),
    b2Vec2(0.057291626835217886, -0.4479166903502308)};
const int asteroid1PointsCount = 12;

const b2Vec2 asteroid2Points[] = {
    b2Vec2(-0.35082873126030634, -0.12154699997812933),
    b2Vec2(-0.0966850749587742, -0.4889503034580926),
    b2Vec2(0.08839780072522405, -0.33977902010144123),
    b2Vec2(0.3149171206617623, -0.430939252270621),
    b2Vec2(0.39226520109252255, -0.1712707476981352),
    b2Vec2(0.5, -0.011049742219900347),
    b2Vec2(0.3895027787841655, 0.2403314552308802),
    b2Vec2(-0.124309385744092, 0.4889503034580926),
    b2Vec2(-0.5, 0.25138121754909754),
    b2Vec2(-0.35082873126030634, -0.12154699997812933)};
const int asteroid2PointsCount = 10;

const b2Vec2 asteroid3Points[] = {
    b2Vec2(-0.0821428593025872, -0.2839285783802025),
    b2Vec2(0.21428571293588303, -0.40535714683914514),
    b2Vec2(0.5000000000000001, -0.0875000089763781),
    b2Vec2(0.2214285476715408, 0.40535714683914514),
    b2Vec2(-0.3642857148256468, 0.3839285953880768),
    b2Vec2(-0.5, -0.05892857554555684),
    b2Vec2(-0.23928572899887535, -0.3660714140607422),
    b2Vec2(-0.0821428593025872, -0.2839285783802025)};
const int asteroid3PointsCount = 8;