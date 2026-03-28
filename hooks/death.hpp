#pragma once
#include "../sdks/structs.hpp"
#include "../sdks/engine.hpp"
#include "../sdks/game.hpp"
#include "../config/config.hpp"
#include <random>

// laziness prevails
inline int random_num(int min, int max)
{
    std::random_device rand;
    std::mt19937 gen(rand());
    std::uniform_int_distribution<> range(min, max);
    return range(gen);
}

inline void(*ohandle_on_death)(ashooter_character*, udamage_response*);
inline void handle_on_death(ashooter_character* actor, udamage_response* response)
{
    if (actor && response) {
        auto local_pawn = local_controller->pawn();
        if (local_pawn) {
            auto instigator = response->get_event_instigator_pawn();
            bool is_own_kill = (instigator && local_pawn == instigator);

            if (!m_config.skins.own_kill || (m_config.skins.own_kill && is_own_kill)) {
                auto death_reaction_component = actor->death_reaction_component();
                if (death_reaction_component) {
                    auto uclass_private_object = uobject::find_object(_(L"CoreUObject.Class"));

                    aeffect_container* finisher;
                    if (m_config.skins.finisher_id == 0)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Afterglow/FXC_Finisher_Afterglow_Victim.FXC_Finisher_Afterglow_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 1)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Afterglow3/FXC_Finisher_Afterglow3_Victim.FXC_Finisher_Afterglow3_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 2)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Antares/FXC_Finisher_Antares_Victim.FXC_Finisher_Antares_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 3)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Ashen/FXC_Finisher_Ashen_Victim.FXC_Finisher_Ashen_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 4)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Atlas/FXC_Finisher_Atlas_Victim.FXC_Finisher_Atlas_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 5)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Champions/FXC_Finisher_Champions_Victim.FXC_Finisher_Champions_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 6)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Champions2/FXC_Finisher_Champion2_Victim.FXC_Finisher_Champion2_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 7)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Champions2023/FXC_Champions23_Finisher_Victim.FXC_Champions23_Finisher_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 8)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Champions2024/FXC_Finisher_Champions2024_Victim.FXC_Finisher_Champions2024_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 9)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/ComicBook/FXC_Finisher_Comicbook_Victim.FXC_Finisher_Comicbook_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 10)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Cyberpunk/FXC_Finisher_Cyberpunk_Victim.FXC_Finisher_Cyberpunk_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 11)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Daedalus/FXC_Finisher_Daedalus1_Victim.FXC_Finisher_Daedalus1_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 12)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Demonstone/FXC_Finisher_Demonstone_Victim.FXC_Finisher_Demonstone_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 13)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Dragon/FXC_Finisher_Dragon_Victim.FXC_Finisher_Dragon_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 14)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Dynasty/FXC_Finisher_Dynasty_Victim.FXC_Finisher_Dynasty_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 15)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Edge/FXC_Finisher_Edge_Victim.FXC_Finisher_Edge_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 16)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Fallen/FXC_Finisher_Fallen_Victim.FXC_Finisher_Fallen_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 17)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Gunslinger/FXC_Finisher_Gunslinger_Victim.FXC_Finisher_Gunslinger_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 18)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Hellfire/FXC_Finisher_Hellfire_Victim.FXC_Finisher_Hellfire_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 19)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/HypeBeast/FXC_Finisher_Hypebeast_Victim.FXC_Finisher_Hypebeast_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 20)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/HypeBeast2/FXC_Hypebeast2_Finihser.FXC_Hypebeast2_Finihser_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 21)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Hypedragon/FXC_Finisher_HypeDragon_Victim.FXC_Finisher_HypeDragon_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 22)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/King/FXC_Finisher_King_Victim.FXC_Finisher_King_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 23)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Legion/FXC_Finisher_Legion_Victim.FXC_Finisher_Legion_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 24)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/MonkeyKing/FXC_MonkeyKing_Finisher_Victim.FXC_MonkeyKing_Finisher_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 25)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Motorbike/FXC_Finisher_Motorbike_Victim.FXC_Finisher_Motorbike_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 26)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Ninja/FXC_Finisher_Ninja_Victim.FXC_Finisher_Ninja_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 27)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Oblivion/FXC_Finisher_Oblivion_Victim.FXC_Finisher_Oblivion_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 28)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Oni/FXC_Finisher_Oni_Victim.FXC_Finisher_Oni_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 29)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Permafrost/FXC_Finisher_Permafrost_Victim.FXC_Finisher_Permafrost_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 30)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Protocol/FXC_Finisher_Protocol_Victim.FXC_Finisher_Protocol_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 31)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/SoL/FXC_Finisher_SoL_VictimPart2.FXC_Finisher_SoL_VictimPart2_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 32)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/SoL2/FXC_Finisher_SoL2_VictimPart2.FXC_Finisher_SoL2_VictimPart2_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 33)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Soulstealer/FXC_Finisher_Soulstealer_Victim.FXC_Finisher_Soulstealer_Victim_C"), 0, uclass_private_object);
                    else if (m_config.skins.finisher_id == 34)
                        finisher = uobject::load_object<aeffect_container>(_(L"/Game/Equippables/Finishers/Sovereign/FXC_Finisher_Sovereign_Victim.FXC_Finisher_Sovereign_Victim_C"), 0, uclass_private_object);

                retry:

                    if (finisher) {
                        if (m_config.skins.last_only) {
                            auto actors = ushooter_blueprint_library::static_class()->find_all_shooter_characters_with_alliance<apawn>(local_controller, local_controller);
                            if (actors.size() && actors.size() <= 1) {
                                death_reaction_component->set_death_montage_effect_override(finisher);
                                death_reaction_component->set_death_montage_effect_override_context(local_pawn);

                                death_reaction_component->play_finisher_effect();

                                death_reaction_component->set_death_montage_effect_override(nullptr);
                                death_reaction_component->set_death_montage_effect_override_context(nullptr);
                            }
                        }
                        else {
                            death_reaction_component->set_death_montage_effect_override(finisher);
                            death_reaction_component->set_death_montage_effect_override_context(local_pawn);

                            death_reaction_component->play_finisher_effect();

                            death_reaction_component->set_death_montage_effect_override(nullptr);
                            death_reaction_component->set_death_montage_effect_override_context(nullptr);
                        }

                        if (m_config.skins.randomizer) {
                            m_config.skins.finisher_id = random_num(0, 34);
                        }
                    }
                    else {
                        // filter until valid finisher is found - push invalid to list
                        /*log::push(system::static_class()->get_object_name(finisher).c_str());*/
                        if (m_config.skins.randomizer) {
                            m_config.skins.finisher_id = random_num(0, 34);
                        }

                        goto retry;
                    }
                }
            }
        }
    }

	return spoof_call(ohandle_on_death, actor, response);
}