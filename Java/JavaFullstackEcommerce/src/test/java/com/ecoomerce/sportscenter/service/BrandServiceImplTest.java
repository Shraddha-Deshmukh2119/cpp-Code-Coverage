package com.ecoomerce.sportscenter.service;

import com.ecoomerce.sportscenter.entity.Brand;
import com.ecoomerce.sportscenter.model.BrandResponse;
import com.ecoomerce.sportscenter.repository.BrandRepository;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class BrandServiceImplTest {

    @Mock
    private BrandRepository brandRepository;

    @InjectMocks
    private BrandServiceImpl brandService;

    @Test
    void getAllBrands_returnsMappedList() {
        when(brandRepository.findAll()).thenReturn(List.of(
                Brand.builder().id(1).name("Nike").build(),
                Brand.builder().id(2).name("Adidas").build()
        ));

        List<BrandResponse> result = brandService.getAllBrands();

        assertThat(result).hasSize(2);
        assertThat(result.get(0).getName()).isEqualTo("Nike");
        assertThat(result.get(1).getId()).isEqualTo(2);
    }
}
