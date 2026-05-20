package com.ecoomerce.sportscenter.service;

import com.ecoomerce.sportscenter.entity.Type;
import com.ecoomerce.sportscenter.model.TypeResponse;
import com.ecoomerce.sportscenter.repository.TypeRepository;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import java.util.List;

import static org.assertj.core.api.Assertions.assertThat;
import static org.mockito.Mockito.when;

@ExtendWith(MockitoExtension.class)
class TypeServiceImplTest {

    @Mock
    private TypeRepository typeRepository;

    @InjectMocks
    private TypeServiceImpl typeService;

    @Test
    void getAllTypes_returnsMappedList() {
        when(typeRepository.findAll()).thenReturn(List.of(
                Type.builder().id(1).name("Shoes").build(),
                Type.builder().id(2).name("Balls").build()
        ));

        List<TypeResponse> result = typeService.getAllTypes();

        assertThat(result).hasSize(2);
        assertThat(result.get(0).getName()).isEqualTo("Shoes");
        assertThat(result.get(1).getId()).isEqualTo(2);
    }
}
