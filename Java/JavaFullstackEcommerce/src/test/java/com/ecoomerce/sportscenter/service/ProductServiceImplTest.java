package com.ecoomerce.sportscenter.service;

import com.ecoomerce.sportscenter.entity.Brand;
import com.ecoomerce.sportscenter.entity.Product;
import com.ecoomerce.sportscenter.entity.Type;
import com.ecoomerce.sportscenter.exceptions.ProductNotFoundException;
import com.ecoomerce.sportscenter.model.ProductResponse;
import com.ecoomerce.sportscenter.repository.ProductRepository;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageImpl;
import org.springframework.data.domain.PageRequest;

import java.util.List;
import java.util.Optional;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class ProductServiceImplTest {

    @Mock
    private ProductRepository productRepository;

    @InjectMocks
    private ProductServiceImpl productService;

    @Test
    void getProductById_returnsProductResponse() {
        Product product = sampleProduct(1);
        when(productRepository.findById(1)).thenReturn(Optional.of(product));

        ProductResponse response = productService.getProductById(1);

        assertThat(response.getId()).isEqualTo(1);
        assertThat(response.getName()).isEqualTo("Ball");
        assertThat(response.getProductBrand()).isEqualTo("Nike");
        assertThat(response.getProductType()).isEqualTo("Shoes");
    }

    @Test
    void getProductById_throwsWhenNotFound() {
        when(productRepository.findById(99)).thenReturn(Optional.empty());

        assertThatThrownBy(() -> productService.getProductById(99))
                .isInstanceOf(ProductNotFoundException.class)
                .hasMessageContaining("doesn't exist");
    }

    @Test
    void getProducts_returnsPagedResponses() {
        Page<Product> page = new PageImpl<>(List.of(sampleProduct(1)), PageRequest.of(0, 10), 1);
        when(productRepository.findAll(PageRequest.of(0, 10))).thenReturn(page);

        Page<ProductResponse> result = productService.getProducts(PageRequest.of(0, 10));

        assertThat(result.getContent()).hasSize(1);
        assertThat(result.getContent().get(0).getName()).isEqualTo("Ball");
    }

    @Test
    void searchProductsByName_returnsMappedList() {
        when(productRepository.searchByName("ball")).thenReturn(List.of(sampleProduct(1)));

        List<ProductResponse> result = productService.searchProductsByName("ball");

        assertThat(result).hasSize(1);
        verify(productRepository).searchByName("ball");
    }

    @Test
    void searchProductsByBrand_returnsMappedList() {
        when(productRepository.searchByBrand(2)).thenReturn(List.of(sampleProduct(1)));

        List<ProductResponse> result = productService.searchProductsByBrand(2);

        assertThat(result).hasSize(1);
        verify(productRepository).searchByBrand(2);
    }

    @Test
    void searchProductsByType_returnsMappedList() {
        when(productRepository.searchByType(3)).thenReturn(List.of(sampleProduct(1)));

        List<ProductResponse> result = productService.searchProductsByType(3);

        assertThat(result).hasSize(1);
        verify(productRepository).searchByType(3);
    }

    @Test
    void searchProductsByBrandandType_returnsMappedList() {
        when(productRepository.searchByBrandAndType(2, 3)).thenReturn(List.of(sampleProduct(1)));

        List<ProductResponse> result = productService.searchProductsByBrandandType(2, 3);

        assertThat(result).hasSize(1);
        verify(productRepository).searchByBrandAndType(2, 3);
    }

    @Test
    void searchProductsByBrandTypeAndName_returnsMappedList() {
        when(productRepository.searchByBrandTypeAndName(2, 3, "ball"))
                .thenReturn(List.of(sampleProduct(1)));

        List<ProductResponse> result = productService.searchProductsByBrandTypeAndName(2, 3, "ball");

        assertThat(result).hasSize(1);
        verify(productRepository).searchByBrandTypeAndName(2, 3, "ball");
    }

    private Product sampleProduct(int id) {
        Brand brand = Brand.builder().id(2).name("Nike").build();
        Type type = Type.builder().id(3).name("Shoes").build();
        return Product.builder()
                .id(id)
                .name("Ball")
                .description("Sports ball")
                .price(1000L)
                .pictureUrl("http://img/ball.png")
                .brand(brand)
                .type(type)
                .build();
    }
}
