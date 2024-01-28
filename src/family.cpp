#include "family.h"

namespace game {

const common::Stats EMPTY;
const common::Stats DAD{"Dad (Joke)",            2, 50.0, 5.0, 5.0, 0};
const common::Stats MUM{"Serious Mum",           7, 10.0, 1.0, 1.0, 1};
const common::Stats SISTER{"Cranky Sister",      3, 25.0, 2.0, 1.0, 2};
const common::Stats BROTHER{"Pubescent Brother", 1, 15.0, 3.0, 1.0, 2};
const common::Stats CHILD{"Volatile Child",      6, 10.0, 2.0, 5.0, 3};
const common::Stats UNCLE{"Funny Muscle Uncle",  5, 25.0, 7.0, 4.0, 4};
const common::Stats GRANDPA{"Grandpa",           4, 40.0, 6.0, 3.0, 5};
const common::Stats GRANDMA{"Grandma",           8, 5.0, 3.0, 2.0, 6};

const std::vector<const common::Stats *> ALL{
        &DAD,
        &MUM,
        &SISTER,
        &BROTHER,
        &CHILD,
        &UNCLE,
        &GRANDPA,
        &GRANDMA,
};

} // namespace game
