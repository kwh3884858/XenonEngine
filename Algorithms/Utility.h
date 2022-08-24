template<typename TO, typename FROM>
inline TO MC_CheckedCast(FROM* aFromValue)
{
#if ENABLE_CHECKED_CAST
	assert(!aFromValue || dynamic_cast<TO>(aFromValue) != nullptr, "Invalid static_cast, this will crash and/or cause memory corruption");
#endif
	return static_cast<TO>(aFromValue);
}

template<typename TO, typename FROM>
inline TO MC_CheckedCast(FROM& aFromValue)
{
#if ENABLE_CHECKED_CAST
	assert(dynamic_cast<typename std::add_pointer<TO>::type>(&aFromValue) != nullptr, "Invalid static_cast, this will crash and/or cause memory corruption");
#endif
	return static_cast<TO>(aFromValue);
}
template<typename TO, typename FROM>
inline const TO MC_CheckedCast(const FROM* aFromValue)
{
#if ENABLE_CHECKED_CAST
	assert(!aFromValue || dynamic_cast<const TO>(aFromValue) != nullptr, "Invalid static_cast, this will crash and/or cause memory corruption");
#endif
	return static_cast<const TO>(aFromValue);
}

template<typename TO, typename FROM>
inline const TO MC_CheckedCast(const FROM& aFromValue)
{
#if ENABLE_CHECKED_CAST
	assert(dynamic_cast<typename std::add_pointer<const TO>::type>(&aFromValue) != nullptr, "Invalid static_cast, this will crash and/or cause memory corruption");
#endif
	return static_cast<const TO>(aFromValue);
}