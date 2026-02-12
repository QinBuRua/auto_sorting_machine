#ifndef AUTO_SORTING_MACHINE_INCLUDES_CLASSES_MODELHEADER_INL
#define AUTO_SORTING_MACHINE_INCLUDES_CLASSES_MODELHEADER_INL

template<std::input_iterator Iter>
void ModelHeader::set_sha256_from(Iter begin, Iter end) {
   using picosha2::hash256;
   hash256(begin, end, m_Sha256.begin(), m_Sha256.end());
}

template<std::ranges::input_range Ranges>
void ModelHeader::set_sha256_from(Ranges&& ranges) {
   using picosha2::hash256;
   hash256(std::ranges::begin(ranges),std::ranges::end(ranges), m_Sha256.begin(), m_Sha256.end());
}

template<std::same_as<ModelMatching> ... Matchings>
uint8_t model_match(Matchings... matchings) {
   return (std::to_underlying(matchings) | ...);
}

#endif // AUTO_SORTING_MACHINE_INCLUDES_CLASSES_MODELHEADER_INL
