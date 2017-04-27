//
// Created by dawid on 23.04.17.
//

#include <gtest/gtest.h>
#include <TLSF.hpp>

using namespace PiOS;

class TLSFTest : public ::testing::Test {
public:
    static constexpr std::size_t mMetadataSize = TLSF<>::metadataSize();
    static constexpr std::size_t mBlockHeaderSize = sizeof(void *) + sizeof(std::size_t);
    static constexpr std::size_t mFreeBlockHeaderSize = 3 * sizeof(void *) + sizeof(std::size_t);
};

const std::size_t TLSFTest::mMetadataSize;
const std::size_t TLSFTest::mBlockHeaderSize;
const std::size_t TLSFTest::mFreeBlockHeaderSize;


TEST_F(TLSFTest, impossibleAllocationsTest) {
    constexpr auto bufferSize = PiOS::pow(1024, 2);
    char *buffer = static_cast<char *>(::operator new(bufferSize));

    TLSF<> tlsf(MemorySpace(buffer, buffer + bufferSize));
    auto space = tlsf.allocate(PiOS::pow(1024, 3));

    ASSERT_EQ(space.begin(), nullptr);
    ASSERT_EQ(space.end(), nullptr);
    ASSERT_EQ(space.size(), 0);

    space = tlsf.allocate(0);
    ASSERT_EQ(space.begin(), nullptr);
    ASSERT_EQ(space.end(), nullptr);
    ASSERT_EQ(space.size(), 0);

    ::operator delete(buffer);
}

TEST_F(TLSFTest, allocationsOnlyTest) {
    constexpr auto bufferSize = PiOS::pow(1024, 2);
    char *buffer = static_cast<char *>(::operator new(bufferSize));

    TLSF<> tlsf(MemorySpace(buffer, buffer + bufferSize));

    auto space = tlsf.allocate(1);
    auto expectedBlockBegin = buffer + mMetadataSize + mBlockHeaderSize;
    auto expectedBlockEnd = expectedBlockBegin + mFreeBlockHeaderSize;
    ASSERT_EQ(space.begin(), expectedBlockBegin);
    ASSERT_EQ(space.end(), expectedBlockEnd);
    ASSERT_GE(space.size(), mFreeBlockHeaderSize);

    space = tlsf.allocate(mFreeBlockHeaderSize);
    expectedBlockBegin = expectedBlockEnd + mBlockHeaderSize;
    expectedBlockEnd = expectedBlockBegin + mFreeBlockHeaderSize;
    ASSERT_EQ(space.begin(), expectedBlockBegin);
    ASSERT_EQ(space.end(), expectedBlockEnd);
    ASSERT_EQ(space.size(), mFreeBlockHeaderSize);

    space = tlsf.allocate(2 * mFreeBlockHeaderSize);
    expectedBlockBegin = expectedBlockEnd + mBlockHeaderSize;
    expectedBlockEnd = expectedBlockBegin + 2 * mFreeBlockHeaderSize;
    ASSERT_EQ(space.begin(), expectedBlockBegin);
    ASSERT_EQ(space.end(), expectedBlockEnd);
    ASSERT_EQ(space.size(), 2 * mFreeBlockHeaderSize);

    std::size_t allocationSize = 97; //quite random
    space = tlsf.allocate(allocationSize);
    expectedBlockBegin = expectedBlockEnd + mBlockHeaderSize;
    expectedBlockEnd = expectedBlockBegin + allocationSize;
    ASSERT_EQ(space.begin(), expectedBlockBegin);
    ASSERT_EQ(space.end(), expectedBlockEnd);
    ASSERT_EQ(space.size(), allocationSize);

    allocationSize = 1024;
    space = tlsf.allocate(allocationSize);
    expectedBlockBegin = expectedBlockEnd + mBlockHeaderSize;
    expectedBlockEnd = expectedBlockBegin + allocationSize;
    ASSERT_EQ(space.begin(), expectedBlockBegin);
    ASSERT_EQ(space.end(), expectedBlockEnd);
    ASSERT_EQ(space.size(), allocationSize);

    ::operator delete(buffer);
}

TEST_F(TLSFTest, zeroBalanceTest) {
    constexpr auto bufferSize = PiOS::pow(1024, 2);
    char *buffer = static_cast<char *>(::operator new(bufferSize));

    TLSF<> tlsf(MemorySpace(buffer, buffer + bufferSize));

    //should influence nothing
    auto emptySpace = tlsf.allocate(0);
    tlsf.deallocate(emptySpace.begin());
    emptySpace = tlsf.allocate(PiOS::pow(1024, 3)); // space too big
    tlsf.deallocate(emptySpace.begin());

    for (auto size : {1039u, 1024u, 10u}) {
        std::size_t allocationSize = size;
        auto space = tlsf.allocate(allocationSize);
        tlsf.deallocate(space.begin());
        auto compareSpace = tlsf.allocate(allocationSize);
        ASSERT_EQ(space.begin(), compareSpace.begin());
        ASSERT_EQ(space.size(), compareSpace.size());
        ASSERT_EQ(space.end(), compareSpace.end());
        tlsf.deallocate(space.begin());
    }

    ::operator delete(buffer);
}

TEST_F(TLSFTest, allocateBlocksSameFli) {
    constexpr auto bufferSize = PiOS::pow(1024, 2);
    char *buffer = static_cast<char *>(::operator new(bufferSize));
    TLSF<> tlsf(MemorySpace(buffer, buffer + bufferSize));

    const auto fli = 6u;

    std::bitset<32> sizeToAllocate(0);
    sizeToAllocate.set(fli);
    sizeToAllocate.set(fli - 1);

    std::bitset<32> smallerSizeToAllocate(0);
    smallerSizeToAllocate.set(fli);
    smallerSizeToAllocate.set(fli - 2);

    auto biggerSpace = tlsf.allocate(sizeToAllocate.to_ulong());
    tlsf.allocate(47); // random enough;

    tlsf.deallocate(biggerSpace.begin());
    auto smallerSpace = tlsf.allocate(smallerSizeToAllocate.to_ulong());

    ASSERT_EQ(smallerSpace.begin(), biggerSpace.begin());

    ::operator delete(buffer);
}

TEST_F(TLSFTest, allocateBlocksSameSize) {
    constexpr auto bufferSize = PiOS::pow(1024, 2);
    char *buffer = static_cast<char *>(::operator new(bufferSize));
    TLSF<> tlsf(MemorySpace(buffer, buffer + bufferSize));

    const auto fli = 6u; //good enough, isn't it?

    std::bitset<32> sizeToAllocate(0);
    sizeToAllocate.set(fli);
    sizeToAllocate.set(fli - 1);

    auto space1 = tlsf.allocate(sizeToAllocate.to_ulong());
    auto randomAlloc = tlsf.allocate(47); // random enough;

    tlsf.deallocate(space1.begin());
    auto space2 = tlsf.allocate(sizeToAllocate.to_ulong());

    ASSERT_NE(space2.begin(), space1.begin());
    ASSERT_EQ(space2.begin(), static_cast<char *>(randomAlloc.end()) + mBlockHeaderSize);

    ::operator delete(buffer);
}

TEST_F(TLSFTest, mergingBlocksTest) {
    constexpr auto bufferSize = PiOS::pow(1024, 2);
    char *buffer = static_cast<char *>(::operator new(bufferSize));
    TLSF<> tlsf(MemorySpace(buffer, buffer + bufferSize));

    const auto allocationSize = 1024;

    auto firstBlock = tlsf.allocate(allocationSize);
    auto secondBlock = tlsf.allocate(allocationSize);
    tlsf.allocate(47); //random allocation

    tlsf.deallocate(firstBlock.begin());
    tlsf.deallocate(secondBlock.begin());

    std::size_t smallerAllocation = allocationSize >> 1 | allocationSize >> 2; // changling fli size
    auto checkAllocation = tlsf.allocate(smallerAllocation);
    ASSERT_EQ(checkAllocation.begin(), firstBlock.begin());
    ASSERT_EQ(checkAllocation.end(), static_cast<char *>(firstBlock.begin()) + smallerAllocation);
    ASSERT_EQ(checkAllocation.size(), smallerAllocation);


    ::operator delete(buffer);
}
