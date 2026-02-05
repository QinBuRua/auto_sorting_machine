#ifndef MODELHEADER_INL
#define MODELHEADER_INL

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

#endif // MODELHEADER_INL
