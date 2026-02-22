// #include "CardManager.h"

// #include "Rules/InitialSetup.h"

// CardManager::CardManager(const InitialSetup& initialSetup)
// {
//     for(const auto& colourSetup : initialSetup.mCardsPerColour)
//     {
//         const auto colour = colourSetup.first;
//         for(int cardPower = 1; cardPower <= colourSetup.second; ++cardPower)
//         {
//             mAllCards.emplace_back(cardPower, colour);
//         }
//     }
// }